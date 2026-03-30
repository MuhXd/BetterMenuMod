#include "DragSetting.hpp"
#include "../RFMMenu.hpp"
#include "../RFMNode.hpp"
#include "DragThingy.hpp"
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/ui/Layout.hpp>

using namespace geode::prelude;

#define NumberInput(text, fun, max, min)                                     \
	text->setCommonFilter(geode::CommonFilter::Float);                       \
	text->setCallback([text, this, thing](std::string const &e) {            \
		if (e.empty() || e == "." || e.back() == '.') {                      \
			return;                                                          \
		}                                                                    \
                                                                             \
		double value = geode::utils::numFromString<double>(e).unwrapOr(min); \
		value = std::clamp(value, min, max);                                 \
		fun                                                                  \
                                                                             \
		    std::ostringstream oss;                                          \
		oss << std::fixed << std::setprecision(4) << value;                  \
                                                                             \
		std::string str = oss.str();                                         \
                                                                             \
		str.erase(str.find_last_not_of('0') + 1, std::string::npos);         \
		if (!str.empty() && str.back() == '.') {                             \
			str.pop_back();                                                  \
		}                                                                    \
                                                                             \
		text->setString(str, false);                                         \
	});

matjson::Value matjson::Serialize<Positions>::toJson(Positions const &value) {
	matjson::Value nodes = matjson::makeObject({});

	for (auto const &[nodeId, pos] : value.m_nodes) {
		nodes[nodeId] = matjson::makeObject({{"x", pos.m_x},
		                                     {"y", pos.m_y},
		                                     {"scaleX", pos.m_scaleX},
		                                     {"scaleY", pos.m_scaleY},
											{"rotation", pos.m_rotation},
											{"layout", (int)pos.m_layout}});
	}

	return nodes;
};

Result<Positions> matjson::Serialize<Positions>::fromJson(matjson::Value const &value) {
	Positions result = Positions();

	for (auto const &[nodeId, nodeValue] : value) {
		if (!nodeValue.contains("x"))
			continue;
		if (!nodeValue.contains("y"))
			continue;

		GEODE_UNWRAP_INTO(auto x, nodeValue["x"].asDouble());
		GEODE_UNWRAP_INTO(auto y, nodeValue["y"].asDouble());

		double scaleX = 1;
		double scaleY = 1;
		double rot = 0;
		int layout = (int)ToggleState::Auto;

		if (nodeValue.contains("scaleX")) {
			GEODE_UNWRAP_INTO(scaleX, nodeValue["scaleX"].asDouble());
		};

		if (nodeValue.contains("scaleY")) {
			GEODE_UNWRAP_INTO(scaleY, nodeValue["scaleY"].asDouble());
		};
		if (nodeValue.contains("rotation")) {
			GEODE_UNWRAP_INTO(rot, nodeValue["rotation"].asDouble());
		};
		if (nodeValue.contains("layout")) {
			GEODE_UNWRAP_INTO(layout, nodeValue["layout"].asInt());
		};

		result[nodeId] = NodeData(x, y, scaleX, scaleY, rot, layout);
	}
	return Ok(result);
}

void NodeData::affectNode(CCNode *node, CanNodeData options) {
	return NodeData::affectNode(CCDirector::get()->getWinSize(), node, options);
};
#define LayoutChange(type)if (type* AxisLayoutItem = typeinfo_cast<type*>(node->getLayout())){ \
			AxisLayoutItem->setAxis(layoutScan() ? geode::Axis::Column : geode::Axis::Row); \
			node->updateLayout(); \
		};
void NodeData::affectNode(cocos2d::CCSize Size, CCNode *node, CanNodeData options) {
	auto pos = node->getPosition();
	log::debug("{}:|{} {}",node->getID(),Size.width / pos.x, Size.height / pos.y);
	node->setPosition(ccp(Size.width * (this->m_x / 1000), Size.height * (this->m_y / 1000)));
	if (options.m_canRotate) node->setRotation(this->m_rotation);
	if (options.m_canScale){ node->setScaleX(this->m_scaleX); node->setScaleY(this->m_scaleY); };
	if (options.m_layoutEditable){ 
		LayoutChange(AxisLayout)
		LayoutChange(SimpleAxisLayout)
	};
};

Result<std::shared_ptr<SettingV3>> DraggableSetting::parse(std::string const &key, std::string const &modID, matjson::Value const &json) {
	auto res = std::make_shared<DraggableSetting>();
	auto root = checkJson(json, "DraggableSetting");
	res->parseBaseProperties(key, modID, root);
	if (auto value = root.has("Configs")) {
		for (auto &[str, val] : value.properties()) {
#define Import(type, Vname, def)          \
	type Vname = def;                     \
	if (auto exportd = val.has(#Vname)) { \
		exportd.into(Vname);              \
	};

			Import(
			    bool, CanRotate, false)
			    Import(
			        bool, CanScale, false)
			        Import(
			            double, ContentSizeY, 20)
			            Import(
			                double, ContentSizeX, 104)

			                Import(
			                    bool, LayoutEditable, false)

			                    log::debug("added\nname: {}, CanRotate {}, CanScale {}, ContentSizeY {}, ContentSizeX {}, LayoutEditable {}", str, CanRotate, CanScale, ContentSizeY, ContentSizeX, LayoutEditable);
			res->m_config[str] = CanNodeData(CanRotate, CanScale, ContentSizeX, ContentSizeY, LayoutEditable);
		}
	};

	root.checkUnknownKeys();
	return root.ok(std::static_pointer_cast<SettingV3>(res));
}
class DraggableSettingPopup : public geode::Popup {
  protected:
	const float const_mmx = 45 / 2;
	const float const_mmy = 44 / 2;
	const float const_mmoffset = 15.0f;
	NodeData m_SelectedNode;
	DragThingy *m_currentDraggy = nullptr;
	geode::NineSlice *m_currentDraggySprite = nullptr;
	bool currentToggle = false;
	geode::NineSlice *m_bg;
	double m_snapPoint = 0;
	RFMMenu *m_leftMenu;
	RFMMenu *m_rightMenu;
	RFMMenu *m_groupMenu;
	CanNodeData m_CanNodeData;
	std::shared_ptr<DraggableSetting> m_setting;
	DraggableSettingNode *m_settingNode;
	inline CCMenuItem *SpriteExtraChild(CCMenu *menu, const char *id, CCNode *ccnode, auto fun) {
		auto btn = CCMenuItemExt::createSpriteExtra(ccnode, fun);
		btn->setID(id);
		menu->addChild(btn);
		return btn;
	};
	inline CCMenuItem *SpriteExtraChild(CCMenu *menu, const char *id, float size, CCNode *ccnode, auto fun) {
		ccnode->setScale(size);
		return SpriteExtraChild(menu, id, ccnode, fun);
	};
	CCPoint ScaledSnap(CCPoint offset) {
		return ScaledSnap(offset, m_currentDraggySprite);
	};
	CCPoint ScaledSnap(CCPoint offset, CCNode *drag) {
		auto size = m_bg->getContentSize();
		return ccp((size.width - drag->getContentWidth() / 2) * (offset.x / 1000), (size.height - drag->getContentHeight() / 2) * (offset.y / 1000));
	};
	void sendSave(auto id,CCNode* item, std::pair<double,double> position) {
		auto g2 = m_settingNode->getValue();
			if (auto g = g2.get(id)) {
				if (g.has_value()) {
					auto val = g.value();
					val.m_x = position.first * 1000;
					val.m_y = position.second * 1000;
					val.m_scaleY =  item->getScaleY();
					val.m_scaleX =  item->getScaleX();
					val.m_rotation = item->getRotation();
					val.m_layout = m_SelectedNode.m_layout;
					g2[id] = val;
				};
			};
			m_settingNode->setValue(g2, nullptr);
	};

	std::pair<double,double> RecalculatePoint(CCNode* item,std::string id, bool cbf = false, bool Move = true) {
		auto parentSize = m_bg->getContentSize();
		auto parentPos  = m_bg->getPosition();
		auto anchor     = m_bg->getAnchorPoint();
		auto bbox = item->boundingBox();
		float halfW = bbox.size.width  / 2.f;
		float halfH = bbox.size.height / 2.f;
		float minX = parentPos.x - parentSize.width  * anchor.x + halfW;
		float maxX = parentPos.x - parentSize.width  * anchor.x + parentSize.width  - halfW;
		float minY = parentPos.y - parentSize.height * anchor.y + halfH;
		float maxY = parentPos.y - parentSize.height * anchor.y + parentSize.height - halfH;
		auto pos = item->getPosition();
		pos.x = std::clamp(pos.x, minX, maxX);
		pos.y = std::clamp(pos.y, minY, maxY);
		double percentX = (pos.x - minX) / (maxX - minX);
		double percentY = (pos.y - minY) / (maxY - minY);
		if (cbf) {
			double div = m_snapPoint / 1000;
			percentX = std::round(percentX / div) * div;
			percentY = std::round(percentY / div) * div;
		}
		std::pair<double,double> ret;
		if (Move) {
			double x = minX + percentX * (maxX - minX);
			double y = minY + percentY * (maxY - minY);
			minX -= halfW;
			maxX += halfW;
			minY -= halfH;
			maxY += halfH;
			ret = {(pos.x - minX) / (maxX - minX), (pos.y - minY) / (maxY - minY)};
			item->setPosition(x,y);
			if (m_CanNodeData.m_layoutEditable) {
				m_SelectedNode.m_x = ret.first * 1000;
				m_SelectedNode.m_y = ret.second * 1000;
				bool vir = m_SelectedNode.layoutScan();
				if (!vir) {
					m_currentDraggy->setContentSize({m_CanNodeData.m_contentSizeX,m_CanNodeData.m_contentSizeY});
					m_currentDraggySprite->setContentSize(m_currentDraggy->getContentSize());
				} else {
					m_currentDraggy->setContentSize({m_CanNodeData.m_contentSizeY,m_CanNodeData.m_contentSizeX});
					m_currentDraggySprite->setContentSize(m_currentDraggy->getContentSize());
				}
			}
		}else{
			minX -= halfW;
			maxX += halfW;
			minY -= halfH;
			maxY += halfH;
			ret = {(pos.x - minX) / (maxX - minX), (pos.y - minY) / (maxY - minY)};
		}
		
		
		if (!id.empty()) {
			sendSave(id,item, ret);
		};
		return ret;
	};
	void sendSave(std::string id) {
		RecalculatePoint(m_currentDraggy, id, false, true);
	}
	bool init(std::shared_ptr<DraggableSetting> attachment, DraggableSettingNode *node) {
		m_size = CCSize{420, 270};
		if (!geode::Popup::init(
		        m_size.width,
		        m_size.height,
		        "GJ_square04.png")) {
			return false;
		}
		m_settingNode = node;
		m_groupMenu = RFMMenu::create();
		m_groupMenu->ignoreAnchorPointForPosition(false);
		m_groupMenu->setPosition(m_mainLayer->getPosition());
		m_groupMenu->setContentSize(m_mainLayer->getContentSize());
		m_groupMenu->setPosition(m_size / 2);

		auto winSize = CCDirector::get()->getWinSize();
		m_setting = attachment;

		m_leftMenu = RFMMenu::create();
		m_leftMenu->setZOrder(99);
		m_leftMenu->setAnchorPoint({0, 1});
		m_leftMenu->setContentSize({41, m_size.height - 30});
		m_leftMenu->setLayout(
		    SimpleColumnLayout::create()
		        ->setGap(5.f)
		        ->setCrossAxisScaling(AxisScaling::ScaleDownGaps)
		        ->setCrossAxisAlignment(geode::CrossAxisAlignment::Center));

		m_rightMenu = RFMMenu::create();
		m_rightMenu->setZOrder(99);
		m_rightMenu->setAnchorPoint({1, 1});
		m_rightMenu->setContentSize({41, m_size.height - 30});
		m_rightMenu->setLayout(
		    SimpleColumnLayout::create()
		        ->setGap(10.f)
		        ->setCrossAxisScaling(AxisScaling::ScaleDownGaps)
		        ->setCrossAxisAlignment(geode::CrossAxisAlignment::Center));

		m_bg = geode::NineSlice::createWithSpriteFrameName("NeonSquare.png"_spr);
		m_bg->setContentSize(m_size / 1.3);
		CCPoint mainLayerPosition = m_bgSprite->getPosition() - ccp(0, 10);
		m_bg->setPosition(mainLayerPosition);
		m_mainLayer->addChild(m_bg);
		m_mainLayer->addChild(m_groupMenu);
		m_groupMenu->addChildAtPosition(m_leftMenu, Anchor::TopLeft, {5.f, -24.f});
		m_groupMenu->addChildAtPosition(m_rightMenu, Anchor::TopRight, {-5.f, -24.f});
		if (auto setting = m_setting.get()) {
			this->setTitle(fmt::format("{} editor", m_setting->getDisplayName()));
			EditorButtons("");
			auto clipper = CCClippingNode::create();
			m_mainLayer->addChild(clipper);
			clipper->setStencil(m_bg);
			for (auto const &[id, node] : setting->m_config) {
				auto NeonSquare = geode::NineSlice::createWithSpriteFrameName("NeonSquare.png"_spr);
				NeonSquare->ignoreAnchorPointForPosition(true);
				if (node.m_layoutEditable) {
					bool vir = m_settingNode->getValue().get(id).value_or(NodeData()).layoutScan();
					if (!vir) {
						NeonSquare->setContentSize({node.m_contentSizeX,node.m_contentSizeY});
					} else {
						NeonSquare->setContentSize({node.m_contentSizeY,node.m_contentSizeX});
					}
				}
				auto draggot = DragThingy::create([=](DragThingy *item) {
					if (m_currentDraggySprite) m_currentDraggySprite->setColor({255,255,255});
					NeonSquare->setColor({ 140, 0, 255 });
					m_currentDraggy = item;
					m_currentDraggySprite = NeonSquare;
					EditorButtons(id); }, [this, setting, id](cocos2d::CCPoint delta, DragThingy *item) {
					if (!item || !m_bg) return; 
					auto pos = item->getPosition() + delta;
					item->setPosition(pos);
					sendSave(id); }, [this,NeonSquare, id](DragThingy *item) {
						 NeonSquare->setColor({255, 0, 157});
						 if (!currentToggle && m_snapPoint > 0) {
							RecalculatePoint(item, id, true, true);
						 };
				 });

				draggot->setContentSize(NeonSquare->getContentSize());
				clipper->addChild(draggot);
				draggot->addChild(NeonSquare);

				auto value = m_settingNode->getValue().get(id).value_or(NodeData());
				auto size = m_bg->getContentSize();
				auto anchor = m_bg->getAnchorPoint();
				float minX = m_bg->getPositionX() - size.width * anchor.x;
				float maxX = minX + size.width ;
				float minY = m_bg->getPositionY() - size.height * anchor.y;
				float maxY = minY + size.height;
				auto x = minX + ((maxX - minX) * (value.m_x / 1000));
				auto y = minY + ((maxY - minY) * (value.m_y / 1000));
				draggot->setPosition(x,y);
				draggot->setScaleX(value.m_scaleX);
				draggot->setScaleY(value.m_scaleY);
				draggot->setRotation(value.m_rotation);
			}
		} else {
			this->setTitle(fmt::format("failed to get for {}", m_setting->getDisplayName()));
		};

		return true;
	};
	void EditorButtons(std::string thing) {
		m_SelectedNode = NodeData();
		m_CanNodeData = CanNodeData();
		if (!thing.empty()) {
			auto setting = m_setting.get();
			auto CurrentValuePositions = m_settingNode->getValue();
			m_CanNodeData = setting->configGet(thing).value_or(CanNodeData());
			m_SelectedNode = CurrentValuePositions.get(thing).value_or(m_SelectedNode);
		};
		m_leftMenu->removeAllChildren();
		m_rightMenu->removeAllChildren();
		if (!thing.empty()) {
			auto MoveButtons = RFMMenu::create();
			MoveButtons->setContentSize({45, 44});
			SpriteExtraChild(MoveButtons, "up-button"_spr, 0.5, EditorButtonSprite::createWithSpriteFrameName("edit_upBtn_001.png", 0.9, geode::EditorBaseColor::BrightGreen, geode::EditorBaseSize::Normal),
							[this](auto btn) {
								if (m_currentDraggy)
									m_currentDraggy->m_onMove(ScaledSnap(ccp(0, m_snapPoint)), m_currentDraggy);
							})
				->setPosition({const_mmx, const_mmy + const_mmoffset});
			SpriteExtraChild(MoveButtons, "down-button"_spr, 0.5, EditorButtonSprite::createWithSpriteFrameName("edit_downBtn_001.png", 0.9, geode::EditorBaseColor::BrightGreen, geode::EditorBaseSize::Normal),
							[this](auto btn) {
								if (m_currentDraggy)
									m_currentDraggy->m_onMove(ScaledSnap(ccp(0, -m_snapPoint)), m_currentDraggy);
							})
				->setPosition({const_mmx, const_mmy - const_mmoffset});
			SpriteExtraChild(MoveButtons, "right-button"_spr, 0.5, EditorButtonSprite::createWithSpriteFrameName("edit_rightBtn_001.png", 0.9, geode::EditorBaseColor::BrightGreen, geode::EditorBaseSize::Normal),
							[this](auto btn) {
								if (m_currentDraggy)
									m_currentDraggy->m_onMove(ScaledSnap(ccp(m_snapPoint, 0)), m_currentDraggy);
							})
				->setPosition({const_mmx + const_mmoffset, const_mmy});
			;
			m_leftMenu->addChild(MoveButtons);
			SpriteExtraChild(MoveButtons, "left-button"_spr, 0.5, EditorButtonSprite::createWithSpriteFrameName("edit_leftBtn_001.png", 0.9, geode::EditorBaseColor::BrightGreen, geode::EditorBaseSize::Normal),
							[this](auto btn) {
								if (m_currentDraggy)
									m_currentDraggy->m_onMove(ScaledSnap(ccp(-m_snapPoint, 0)), m_currentDraggy);
							})
				->setPosition({const_mmx - const_mmoffset, const_mmy});
			;
		};
		auto gridButton = CCMenuItemExt::createToggler(EditorButtonSprite::createWithSpriteFrameName("edit_snapBtn_001.png",
		                                                                                             0.9, geode::EditorBaseColor::LightBlue, geode::EditorBaseSize::Normal),
		                                               EditorButtonSprite::createWithSpriteFrameName("edit_snapBtn_001.png",
		                                                                                             0.9, geode::EditorBaseColor::BrightGreen, geode::EditorBaseSize::Normal),
		                                               [this](auto btn) {
			                                               currentToggle = btn->m_toggled;
		                                               });
		TextInput *rotation = nullptr;
		if (m_CanNodeData.m_canRotate) {
			rotation = TextInput::create(gridButton->getContentWidth(), "0");
			rotation->setLabel("Rotation");
			NumberInput(rotation, { 
				if (value >= 360) value-=360;
				if (value <= -360) value+=360;
				if (!m_currentDraggy) return;
				m_currentDraggy->setRotation(value);
				sendSave(thing);
			 }, 720.0, -720.0)
			 rotation->setString(std::to_string(m_SelectedNode.m_rotation),true);

			SpriteExtraChild(m_leftMenu, "right-rotate-button"_spr, EditorButtonSprite::createWithSpriteFrameName("edit_cwBtn_001.png", 0.9, geode::EditorBaseColor::BrightGreen, geode::EditorBaseSize::Normal),
			                 [this, rotation](auto btn) {
				                 if (!m_currentDraggy) return;
								 rotation->setString(std::to_string(m_currentDraggy->getRotation() + 45), true);
			                 });
			SpriteExtraChild(m_leftMenu, "left-rotate-button"_spr, EditorButtonSprite::createWithSpriteFrameName("edit_ccwBtn_001.png", 0.9, geode::EditorBaseColor::BrightGreen, geode::EditorBaseSize::Normal),
			                 [this, rotation](auto btn) {
				                 if (!m_currentDraggy) return;
								 rotation->setString(std::to_string(m_currentDraggy->getRotation() - 45), true);
			                 });
		};

		if (m_CanNodeData.m_layoutEditable) {
			auto layout = RFMNode::createWithCallback<ToggleState, ToggleState::Auto, ToggleState::Row, ToggleState::Column>([this, thing](RFMNode *thisx, ToggleState value) {
				m_SelectedNode.m_layout = value;
				sendSave(thing);
			},
			                                                                                                                 EditorButtonSprite::create(CCLabelBMFont::create("Auto", "bigFont.fnt"), geode::EditorBaseColor::Magenta, geode::EditorBaseSize::Normal), EditorButtonSprite::create(CCLabelBMFont::create("Row", "bigFont.fnt"), geode::EditorBaseColor::Pink, geode::EditorBaseSize::Normal), EditorButtonSprite::create(CCLabelBMFont::create("Column", "bigFont.fnt"), geode::EditorBaseColor::DarkGray, geode::EditorBaseSize::Normal));
			layout->setID("layout"_spr);
			layout->setValue(m_SelectedNode.m_layout);
			m_rightMenu->addChild(layout);
		}

		gridButton->toggle(!currentToggle);
		gridButton->setID("grid-button"_spr);
		if (rotation) {
			m_rightMenu->addChild(rotation);
		}
		m_rightMenu->addChild(gridButton);
		TextInput *text = TextInput::create(gridButton->getContentWidth(), "0");
		NumberInput(text, { m_snapPoint = value; }, 1000.0, 0.0) text->setLabel("Snap");
		text->setString(std::to_string(m_snapPoint), true);
		m_rightMenu->addChild(text);

		if (m_CanNodeData.m_canScale) {
			TextInput *scaleX = TextInput::create(gridButton->getContentWidth(), "1");
			scaleX->setLabel("Scale X");
			NumberInput(scaleX, {
				if (!m_currentDraggy) return;
				if (value == 0) return;
				if (value <= 0.05) value = 0.05;
				m_currentDraggy->setScaleX(value);
				sendSave(thing);	
			 }, 3.0, 0.0)
			m_rightMenu->addChild(scaleX);
			scaleX->setString(std::to_string(m_SelectedNode.m_scaleX), true);
			TextInput *scaleY = TextInput::create(gridButton->getContentWidth(), "1");
			scaleY->setLabel("Scale Y");
			NumberInput(scaleY,  {
				if (!m_currentDraggy) return;
				if (value == 0) return;
				if (value <= 0.05) value = 0.05;
				m_currentDraggy->setScaleY(value);
				sendSave(thing);		
				};, 3.0, 0.0)
			m_rightMenu->addChild(scaleY);
			scaleY->setString(std::to_string(m_SelectedNode.m_scaleY), true);
		}

		m_rightMenu->updateLayout();
		m_leftMenu->updateLayout();
	};

  public:
	static DraggableSettingPopup *create(std::shared_ptr<DraggableSetting> attachment, DraggableSettingNode *node) {
		auto ret = new DraggableSettingPopup;
		if (ret && ret->init(attachment, node)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	};
};

void DraggableSettingNode::Popup() {
	DraggableSettingPopup::create(m_setting, this)->show();
};

bool DraggableSettingNode::init(std::shared_ptr<DraggableSetting> setting, float width) {
	if (!SettingValueNodeV3::init(setting, width))
		return false;
	m_buttonSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png",1.1,geode::CircleBaseColor::DarkPurple, geode::CircleBaseSize::Small);
	m_buttonSprite->setScale(.68f);
	m_button = CCMenuItemSpriteExtra::create(m_buttonSprite, this, menu_selector(DraggableSettingNode::Popup));
	m_setting = setting;
	this->getButtonMenu()->addChildAtPosition(m_button, Anchor::Center);
	this->getButtonMenu()->setContentWidth(30);
	this->getButtonMenu()->updateLayout();
	this->updateState(nullptr);
	return true;
};

void DraggableSettingNode::updateState(CCNode *invoker) {
	SettingNodeV3::updateState(invoker);
	auto shouldEnable = this->getSetting()->shouldEnable();
	m_button->setEnabled(shouldEnable);
	m_buttonSprite->setCascadeColorEnabled(true);
	m_buttonSprite->setCascadeOpacityEnabled(true);
	m_buttonSprite->setOpacity(shouldEnable ? 255 : 155);
	m_buttonSprite->setColor(shouldEnable ? ccWHITE : ccGRAY);
};

SettingNodeV3 *DraggableSetting::createNode(float width) {
	return DraggableSettingNode::create(std::static_pointer_cast<DraggableSetting>(shared_from_this()), width);
}

$execute {
	auto ret = Mod::get()->registerCustomSettingType("draggable", &DraggableSetting::parse);
	if (!ret) {
		log::error("Unable to register setting type: {}", ret.unwrapErr());
	}
}