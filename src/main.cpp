#include "RFMNode.hpp"
#include "RFMVisibilityActions.hpp"
#include "RFMVisibilityGroup.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "GeodeSettingsCustom/DragSetting.hpp"
#include "RFMMenu.hpp"
#include <memory>

using namespace geode::prelude;
#define modLoaded(id) \
    if (auto ck = Loader::get()->getInstalledMod(id); ck && ck->shouldLoad())

class $modify(MyMenuLayer, MenuLayer) {
	struct Fields {
		bool m_blockHooks = false;
	};
	static void onModify(auto& self) {
        (void)self.setHookPriority("MenuLayer::keyDown", Priority::Replace);
    }

	static inline bool s_passthrough = false;
	void keyDown(cocos2d::enumKeyCodes key, double timestamp) {
		if (m_fields->m_blockHooks) return MenuLayer::keyDown(key, timestamp);
		if (s_passthrough) {
			MenuLayer::keyDown(key, timestamp);
			s_passthrough = false;
		};
	};
	bool isExitPopup() {
		if (FLAlertLayer* alert = CCScene::get()->getChildByType<FLAlertLayer*>(-1)) {
			return alert->getTag() == 0 && typeinfo_cast<MenuLayer*>(alert->m_alertProtocol);
		}
		return false;
	}
	bool isTopLevel() {
        if (CCIMEDispatcher::sharedDispatcher()->hasDelegate()) return false;

        if (auto handler = typeinfo_cast<CCKeyboardHandler*>(CCKeyboardDispatcher::get()->m_pDelegates->lastObject())) {
            // fix the pointer for comparison
            return typeinfo_cast<CCKeyboardDelegate*>(this) == handler->m_pDelegate;
        }

        return true;
    }

	// custom keybinds is cool
	void defineKeybind(std::string id, CopyableFunction<bool(double)> callback) {
		this->addEventListener(
		    KeybindSettingPressedEventV3(Mod::get(), std::move(id)),
		    [callback = std::move(callback), this](Keybind const &keybind, bool down, bool repeat, double timestamp) {
			    if (down) {
				    if (callback(timestamp)) {
				    	return ListenerResult::Stop;
					};
			    }
			    return ListenerResult::Propagate;
		    });
	}
	

	void passThroughKeyDown(enumKeyCodes key, double timestamp, KeyboardModifier modifiers = KeyboardModifier::None) {
        s_passthrough = true;
        auto d = CCKeyboardDispatcher::get();
        auto alt = d->m_bAltPressed;
        auto shift = d->m_bShiftPressed;
        auto ctrl = d->m_bControlPressed;
        auto cmd = d->m_bCommandPressed;
        d->m_bAltPressed = modifiers & KeyboardModifier::Alt;
        d->m_bShiftPressed = modifiers & KeyboardModifier::Shift;
        d->m_bControlPressed = modifiers & KeyboardModifier::Control;
        d->m_bCommandPressed = modifiers & KeyboardModifier::Super;
        this->keyDown(key, timestamp);
        d->m_bAltPressed = alt;
        d->m_bShiftPressed = shift;
        d->m_bControlPressed = ctrl;
        d->m_bCommandPressed = cmd;
    }

	void addKeybinds() {
		modLoaded("geode.texture-loader") {
			this->defineKeybind("texture-loader-keybind", [this](double timestamp) {
				if (!isTopLevel()) return false;
				VideoOptionsLayer *videoCall = VideoOptionsLayer::create();
					videoCall->retain();
					if (CCNode *node = videoCall->getChildByIDRecursive("geode.texture-loader/texture-loader-button")) {
						if (CCMenuItemSpriteExtra *button = typeinfo_cast<CCMenuItemSpriteExtra *>(node))
							button->activate();
					};
					videoCall->release();
				return true;
			});
		}
		this->defineKeybind("exit-keybind", [this](double timestamp) {
			if (!isTopLevel()) {
				if (!isExitPopup()) return false;
			};
            this->passThroughKeyDown(KEY_Escape, timestamp);
			return true;
        });
	};
	
	void LoadPositions(){
		Mod* CurrentMod = Mod::get();
		if (auto setting = typeinfo_cast<DraggableSetting*>(CurrentMod->getSetting("Main-menu").get())) {
			log::info("test {}",setting->m_config.size());
			// fix restart buttons
			if (auto closeMenu = this->getChildByID("close-menu")) {
 				closeMenu->setContentSize({ 200.f, 50.f });
				closeMenu->updateLayout();
				closeMenu->setAnchorPoint({0.5,0.5});
				closeMenu->setLayout( SimpleRowLayout::create()
                    ->setMainAxisAlignment(MainAxisAlignment::Start)
                    ->setGap(5.f));
			};
			auto currentValue = setting->getValue();
			for (auto const &[id, node] : setting->m_config) {
				auto nodeRec = this->getChildByIDRecursive(id);
				if (!nodeRec) continue;
				auto val = currentValue.get(id);
				if (!val.has_value()) continue;
				log::debug("{}",id);
				val.value().affectNode(nodeRec, node);
			}
		};
	};
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}
		Mod* CurrentMod = Mod::get();
		// queue a disabling safe mode
		geode::Loader::get()->queueInMainThread([CurrentMod] {
			CurrentMod->setSavedValue("safe-mode", false);
		});
		if (CurrentMod->setSavedValue("safe-mode", true)) {
			m_fields->m_blockHooks = true;
			log::error("Disabling Hooks, detected crash");
			geode::Loader::get()->queueInMainThread([this] {
                auto pop = geode::createQuickPopup(
                    "Reforged Menu",
                    "The mod has detected a <cr>crash</c> and <cy>disabled itself</c>. would like it to permanently disable itself?",
                    "No", "Yes",
                    [this](auto me, bool btn2) {
                        if (btn2) {
                        	Mod::get()->setSettingValue<bool>("mod-toggle",false);
                        }
                    }, false
                );
                pop->m_scene = this;
                pop->show();
            });
			CurrentMod->setSavedValue("safe-mode", false);
			return true;
		};
		if (CurrentMod->getSettingValue<bool>("mod-toggle")) {
			m_fields->m_blockHooks = true;
			return true;
		}
		CurrentMod->setSavedValue("safe-mode", true);

		Loader::get()->queueInMainThread([this] {
			this->addKeybinds();
		});

		RFMVisibilityActions *l_visibilityActions = RFMVisibilityActions::create();
		this->addChild(l_visibilityActions);
		cocos2d::CCSize winSize = CCDirector::get()->getWinSize();
		CCMenu *compactMenu = RFMMenu::create();
		compactMenu->setPosition(winSize.width, 0);
		compactMenu->setContentWidth(winSize.width / 2);
		compactMenu->setAnchorPoint({1, 0});
		compactMenu->setScale(0.8f);
		compactMenu->setLayout(
		    RowLayout::create()
		        ->setAxisAlignment(AxisAlignment::End)
		        ->setGrowCrossAxis(false)
		        ->setCrossAxisReverse(true));
		compactMenu->setID("compact-bottom-menu"_spr);

		RFMVisibilityGroup *l_compactHide = RFMVisibilityGroup::create();
		this->addChild(l_compactHide);
		if (CCNode *bottomMenu = this->getChildByID("bottom-menu")) {
			l_compactHide->add(bottomMenu);
			//CurrentMod->getSettingValue<Position>("Bottom-menu-position").affectNode(bottomMenu);
			CCMenuItemSpriteExtra* geodeButton = typeinfo_cast<CCMenuItemSpriteExtra*>(
				this->getChildByIDRecursive("geode.loader/geode-button")
			);
			if (geodeButton) {
				Loader::get()->queueInMainThread([=,this] {
					// checking if some fucker removed this
					if (CCNode* geodeButton = this->getChildByIDRecursive("geode.loader/geode-button")) {
						auto truePos = geodeButton->convertToWorldSpaceAR(CCPointZero);
						if (truePos.x > winSize.width || truePos.x < 0 || truePos.y > winSize.height || truePos.y < 0) {
							log::error("user? Why are you like this!!!");
							auto pop = geode::createQuickPopup(
								"Reforged Menu",
								"The <co>mod</c> has detected the <cp>geode button</c> going <cr>offscreen</c>! Would you like to open <cp>geode</c> <cy>settings</c>?",
								"No", "Yes",
								[this](auto me, bool btn2) {
									if (btn2) {
										geode::openModsList();
									}
								}, false
							);
							pop->m_scene = this;
							pop->show();
						};
					}
				});
			};
			if (CurrentMod->getSettingValue<bool>("geode-discord-icon-button")) {
				if (geodeButton) {
					if (auto icon = geodeButton->getNormalImage()) {
						auto Spr = CCSprite::createWithSpriteFrameName("geode-discord.png"_spr);
						Spr->setScale(0.2f);
						Spr->setPositionX(icon->getPositionX());
						Spr->setPositionY(icon->getPositionY());
						icon->addChild(Spr);
					};
				};
			};
			if (CurrentMod->getSettingValue<bool>("NoNewGroundsButton")) {
				if (auto newgrounds = bottomMenu->getChildByIDRecursive("newgrounds-button")) {
					newgrounds->setVisible(false);
					bottomMenu->updateLayout();
				}
			}
		}
		if (CCNode *MoreGamesMenu = this->getChildByID("more-games-menu")) {
			l_compactHide->add(MoreGamesMenu);
		}
		if (CCNode *toprightmenu = this->getChildByID("top-right-menu")) {
			if (CurrentMod->getSettingValue<bool>("compact-main-menu")) {
				this->addChild(compactMenu);

				// special case for the geode button because it's evil 👿
				CCNode *geode = this->getChildByIDRecursive("geode.loader/geode-button");
				CCSprite *geode_PlaceHolder = CircleButtonSprite::createWithSpriteFrameName( 
				    "geode.loader/geode-logo-outline-gold.png",
				    .95f,
				    CircleBaseColor::Green,
				    CircleBaseSize::MediumAlt);
				if (!geode_PlaceHolder)
					geode_PlaceHolder = ButtonSprite::create("??");
				CCMenuItemSpriteExtra *geodeItemExtra = typeinfo_cast<CCMenuItemSpriteExtra *>(geode);
				CCNode *GeodeSprite = geodeItemExtra->getNormalImage();
				GeodeSprite->retain();
				auto geodebtnmini = CCMenuItemSpriteExtra::create(
				    GeodeSprite, this,
				    geodeItemExtra->m_pfnSelector);
				GeodeSprite->release();
				compactMenu->addChild(geodebtnmini);
				geodeItemExtra->setSprite(geode_PlaceHolder);
				l_compactHide->setVisible(false);
				if (CCNode *profileMenu = this->getChildByIDRecursive("profile-menu")) {
					profileMenu->setPositionY(50);
					if (CCNode *playerUsername = m_profileLabel) {
						playerUsername->setPositionY(85);
					};
					if (CCNode *socialMenu = this->getChildByID("social-media-menu")) {
						socialMenu->setVisible(false);
					};
				};
				l_visibilityActions->add(
				    l_compactHide,
				    [=](CCNode *x) {
					    GeodeSprite->retain();
					    geode_PlaceHolder->retain();
					    if (geodeItemExtra)
						    geodeItemExtra->setNormalImage(GeodeSprite);
					    geodeItemExtra->updateSprite();
					    if (geodebtnmini)
						    geodebtnmini->setSprite(geode_PlaceHolder);
					    GeodeSprite->release();
					    geode_PlaceHolder->release();
				    },
				    [=](CCNode *x) {
					    GeodeSprite->retain();
					    geode_PlaceHolder->retain();
					    if (geodeItemExtra)
						    geodeItemExtra->setSprite(geode_PlaceHolder);
					    if (geodebtnmini)
						    geodebtnmini->setNormalImage(GeodeSprite);
					    geodebtnmini->updateSprite();
					    GeodeSprite->release();
					    geode_PlaceHolder->release();
				    });
				RFMNode *MenuActivator = RFMNode::createSheetWithCallback<bool, false, true>([=](RFMNode *thisx, bool value) {
					if (l_compactHide) {
						l_compactHide->setVisible(value);
						compactMenu->setVisible(!value);
					}
				},
				                                                                             "showicon.png"_spr, "hideicon.png"_spr);
				MenuActivator->SetSpriteScale(0.8);
				toprightmenu->addChild(MenuActivator);
				RFMNode *GDSettings = RFMNode::createSheet(
				    "GJ_optionsBtn_001.png",
				    [this](RFMNode *thisx, std::any p) {
					    this->onOptions(thisx);
				    });
				compactMenu->addChild(GDSettings);
				compactMenu->updateLayout();
			};
			toprightmenu->updateLayout();
		}

		if (auto menu = this->getChildByID("right-side-menu")) {
			if (Mod::get()->getSettingValue<bool>("classic-texture-loader-btn")) {
				RFMNode *TextureLoaderPatch = RFMNode::createSheet(
				    "gj_folderBtn_001.png",
				    [=](RFMNode *thisx, std::any p) {
					    VideoOptionsLayer *videoCall = VideoOptionsLayer::create();
					    videoCall->retain();
					    if (CCNode *node = videoCall->getChildByIDRecursive("geode.texture-loader/texture-loader-button")) {
						    if (CCMenuItemSpriteExtra *button = typeinfo_cast<CCMenuItemSpriteExtra *>(node))
							    button->activate();
					    };
					    videoCall->release();
				    });
				TextureLoaderPatch->setID("texture-loader-button"_spr);
				menu->addChild(TextureLoaderPatch);
			}
			menu->updateLayout();
		}
		LoadPositions();
		return true;
	}
};