#pragma once
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/SettingV3.hpp>
using namespace geode::prelude;
enum ToggleState {
	Auto = 0,
	Row,
	Column 
};


struct CanNodeData {
	float m_contentSizeY = 50;
	float m_contentSizeX = 104;
	bool m_canRotate = false;
	bool m_layoutEditable = false;
	CCPoint m_anchorPoint = ccp(0.5,0.5);
	bool m_canScale = false;
	CanNodeData(bool CanRotate, bool CanScale, float ContentSizeX, float ContentSizeY, bool LayoutEditable, CCPoint anchorPoint)
        : m_canRotate(CanRotate), m_canScale(CanScale), m_contentSizeX(ContentSizeX), m_contentSizeY(ContentSizeY), m_layoutEditable(LayoutEditable), m_anchorPoint(anchorPoint) {};
	CanNodeData(CanNodeData const&) = default;
    CanNodeData() = default;

};

struct NodeData {
	bool l_lastdir = false;
	double m_x = 0;
	double m_y = 0;
	double m_scaleX = 1;
	double m_scaleY = 1;
	double m_rotation = 0;
	ToggleState m_layout = ToggleState::Auto;
	/*
	Returns: true if it is Column and false if it is Row
	*/
	bool layoutScan() {
		if (m_layout == ToggleState::Auto) {
			return (m_x <= 250 || m_x >= 750) && (m_y >= 30 && m_y <= 900);
		} else {
			return m_layout == ToggleState::Column;
		};
	};
	/*
	Returns: true if it is Column and false if it is Row
	*/
	bool layoutScan(CanNodeData config, bool flipped) {
		if (m_layout == ToggleState::Auto) {
			float halfW = (config.m_contentSizeX * 420) * m_scaleX  * config.m_anchorPoint.x;
			float halfH = (config.m_contentSizeY * 270) * m_scaleY * config.m_anchorPoint.y;
			float x = flipped ? m_y : m_x;
			float y = flipped ? m_x : m_y;
			return // edging check
			((x - halfW) <= 250 || (x + halfW) >= 750)
			&& ((y - halfH) >= 30 && (y + halfH) <= 900); // verticaling check
		}
		return m_layout == ToggleState::Column;
	};
	/*
	Returns: true if it is Column and false if it is Row
	*/
	bool layoutScan(CanNodeData config) {
		if (m_layout == ToggleState::Auto) {
			float halfW = (config.m_contentSizeX * 420) * m_scaleX  * config.m_anchorPoint.x;
			float halfH = (config.m_contentSizeY * 270) * m_scaleY * config.m_anchorPoint.y;
			return // edging check
			((m_x - halfW) <= 250 || (m_x + halfW) >= 750)
			&& ((m_y - halfH) >= 30 && (m_y + halfH) <= 900); // verticaling check
		} 
		return m_layout == ToggleState::Column;
	};

    NodeData(double px, double py, double sX, double sY, double r, int m_layout)
        : m_x(px), m_y(py), m_scaleX(sX), m_scaleY(sY), m_rotation(r), m_layout((ToggleState)m_layout) {};
    NodeData(NodeData const&) = default;
    NodeData() = default;
    bool operator==(NodeData const& other) const = default;
	void affectNode(CCNode* node, CanNodeData options);
	void affectNode(cocos2d::CCSize Size, CCNode* node, CanNodeData options);
};

struct Positions {
 	std::unordered_map<std::string, NodeData> m_nodes = {};
   	Positions() : m_nodes() {}
	bool operator==(Positions const& other) const {

		return m_nodes == other.m_nodes;
	};
    NodeData& operator[](std::string const& key) {
        return m_nodes[key];
    };
   std::optional<NodeData> get(std::string const& key) const {
   		auto it = m_nodes.find(key);
		if (it != m_nodes.end()) {
			return it->second;
		}
		return std::nullopt;
	};
    bool contains(std::string const& key) const {
        return m_nodes.find(key) != m_nodes.end();
    };
};

template <>
struct matjson::Serialize<Positions> {
	static matjson::Value toJson(Positions const &value);
	static Result<Positions> fromJson(matjson::Value const &value);
};


class DraggableSetting : public SettingBaseValueV3<Positions> {
  public:
  	std::unordered_map<std::string, CanNodeData> m_config = {};
	std::optional<CanNodeData> configGet(std::string const& key) const {
   		auto it = m_config.find(key);
		if (it != m_config.end()) {
			return it->second;
		}
		return std::nullopt;
	};
	static Result<std::shared_ptr<SettingV3>> parse(std::string const &key, std::string const &modID, matjson::Value const &json);
	SettingNodeV3 *createNode(float width) override;
};

template <>
struct geode::SettingTypeForValueType<Positions> {
	using SettingType = DraggableSetting;
};


class DraggableSettingNode : public SettingValueNodeV3<DraggableSetting> {
  protected:
	CCSprite *m_buttonSprite;
	CCMenuItemSpriteExtra *m_button;
	std::shared_ptr<DraggableSetting> m_setting;
	void Popup();
	bool init(std::shared_ptr<DraggableSetting> setting, float width);

	void updateState(CCNode *invoker) override;

  public:
	static DraggableSettingNode *create(std::shared_ptr<DraggableSetting> setting, float width) {
		auto ret = new DraggableSettingNode();
		if (ret && ret->init(setting, width)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};
