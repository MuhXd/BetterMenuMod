#pragma once
#include <Geode/loader/SettingNode.hpp>
#include <Geode/Geode.hpp>
#include <chrono>
#include <thread>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace geode::prelude;

class Settings;

const int DEFAULT_POS = 4;

// thx gdutils Yippeeeeeeee
struct SettingPosStruct {
    int m_pos;
};

class SettingPosValue;

class SettingPosValue : public SettingValue {
protected:
    int m_pos;
public:
    SettingPosValue(std::string const& key, std::string const& modID, int const& position)
      : SettingValue(key, modID), m_pos(position) {}

    bool load(matjson::Value const& json) override {
        try {
            m_pos = static_cast<int>(json.as<int>());
            return true;
        } catch(...) {
            return false;
        }
    }
    bool save(matjson::Value& json) const override {
        json = static_cast<int>(m_pos);
        return true;
    }
    SettingNode* createNode(float width) override;
    void setPos(int pos) {
        m_pos = pos;
    }
    int getPos() const {
        return m_pos;
    }
};

template<>
struct SettingValueSetter<SettingPosStruct> {
    static SettingPosStruct get(SettingValue* setting) {
        auto posSetting = static_cast<SettingPosValue*>(setting);
        struct SettingPosStruct defaultStruct = { posSetting->getPos() };
        return defaultStruct;
    };
    static void set(SettingPosValue* setting, SettingPosStruct const& value) {
        setting->setPos(value.m_pos);
    };
};

class SettingPosNode : public SettingNode {
protected:
    int m_currentPos;
    CCMenuItemToggler* tlBtn;
    CCMenuItemToggler* trBtn;
    CCMenuItemToggler* blBtn;
    CCMenuItemToggler* brBtn;

    int getActiveCornerTag(int corner) {
        switch (corner) {
            case 1: // Top Left
                return 2004;
            case 2: // Top Right
                return 2005;
            case 3: // Bottom Left
                return 2006;
            case 4: // Bottom Right
            default:
                return 2007;
        }
    }
    int tagToCorner(int tag) {
        switch (tag) {
            case 2004: // Top Left
                return 1;
            case 2005: // Top Right
                return 2;
            case 2006: // Bottom Left
                return 3;
            default:
            case 2007: // Bottom Right
                return 4;
        }
    }
    bool init(SettingPosValue* value, float width) {
        if (!SettingNode::init(value))
            return false;
        m_currentPos = value->getPos();
        this->setContentSize({ width, 70.f });
        auto menu = CCMenu::create();
        CCSprite* toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        CCSprite* toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        toggleOn->setScale(.7F);
        toggleOff->setScale(.7F);
        menu->setPosition(width / 2, 23.f);
        tlBtn = CCMenuItemToggler::create(
            toggleOn,
            toggleOff,
            this,
            menu_selector(SettingPosNode::onCornerClick)
        );
        trBtn = CCMenuItemToggler::create(
            toggleOn,
            toggleOff,
            this,
            menu_selector(SettingPosNode::onCornerClick)
        );
        blBtn = CCMenuItemToggler::create(
            toggleOn,
            toggleOff,
            this,
            menu_selector(SettingPosNode::onCornerClick)
        );
        brBtn = CCMenuItemToggler::create(
            toggleOn,
            toggleOff,
            this,
            menu_selector(SettingPosNode::onCornerClick)
        );
        tlBtn->setPosition({ -100, 35 });
        trBtn->setPosition({ 100, 35 });
        blBtn->setPosition({ -100, -10 });
        brBtn->setPosition({ 100, -10 });

        tlBtn->setTag(getActiveCornerTag(1));
        trBtn->setTag(getActiveCornerTag(2));
        blBtn->setTag(getActiveCornerTag(3));
        brBtn->setTag(getActiveCornerTag(4));
        int currentCorner = m_currentPos;
        tlBtn->toggle(!(tlBtn->getTag() == getActiveCornerTag(currentCorner)));
        trBtn->toggle(!(trBtn->getTag() == getActiveCornerTag(currentCorner)));
        blBtn->toggle(!(blBtn->getTag() == getActiveCornerTag(currentCorner)));
        brBtn->toggle(!(brBtn->getTag() == getActiveCornerTag(currentCorner)));
        
        menu->addChild(tlBtn);
        menu->addChild(trBtn);
        menu->addChild(blBtn);
        menu->addChild(brBtn);

        this->addChild(menu);
        return true;
    }
    void onCornerClick(CCObject* sender) {
        tlBtn->toggle(true);
        trBtn->toggle(true);
        blBtn->toggle(true);
        brBtn->toggle(true);
        m_currentPos = tagToCorner(sender->getTag());
        this->dispatchChanged();
    };
    void onInfoBtn(CCObject* sender) {
        FLAlertLayer::create(
            Mod::get()->getSettingDefinition(this->m_value->getKey())->get<CustomSetting>()->json->get<std::string>("name").c_str(),
            Mod::get()->getSettingDefinition(this->m_value->getKey())->get<CustomSetting>()->json->get<std::string>("description").c_str(),
            "OK"
        )->show();
    }
public:
    void commit() override {
        static_cast<SettingPosValue*>(m_value)->setPos(m_currentPos);
        this->dispatchCommitted();
    }
    bool hasUncommittedChanges() override {
        return m_currentPos != static_cast<SettingPosValue*>(m_value)->getPos();
    }
    bool hasNonDefaultValue() override {
        return m_currentPos != DEFAULT_POS;
    }

    // Geode calls this to reset the setting's value back to default
    void resetToDefault() override {
        tlBtn->toggle(true);
        trBtn->toggle(true);
        blBtn->toggle(true);
        brBtn->toggle(false);
        m_currentPos = DEFAULT_POS;
    }
};


void createHeading(auto thisL, Settings* value) {
         std::string head = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("text");
        auto menu = CCMenu::create();
        auto label = CCLabelBMFont::create(head, "bigFont.fnt");
        label->setScale(0.750);
        label->setPositionX(0);
        menu->setPosition(width / 2, 18.f);
        thisL->addChild(menu);
        menu->addChild(label);
}
class Settings : public SettingValue {
protected:
    std::string m_placeholder;
public:
    Settings(std::string const& key, std::string const& modID, std::string const& placeholder)
      : SettingValue(key, modID), m_placeholder(placeholder) {}

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }
    SettingNode* createNode(float width) override;
};


class SettingsNode : public SettingNode {
protected:
 bool init(Settings* value, float width) {
        if (!SettingNode::init(value))
            return false;
        this->setContentSize({ width, 35.f });
            std::string Mode = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("mode");
        if(Mode == "Heading") {
            createHeading(this,value)
        };
        
        return true;
    }

public:
    void commit() override {
        // Let the UI know you have committed the value
        this->dispatchCommitted();
    }

    // Geode calls this to query if the setting value has been changed, 
    // and those changes haven't been committed
    bool hasUncommittedChanges() override {
        return false;
    }

    // Geode calls this to query if the setting has a value that is 
    // different from its default value
    bool hasNonDefaultValue() override {
        return true;
    }

    // Geode calls this to reset the setting's value back to default
    void resetToDefault() override {

    }
    static SettingsNode* create(Settings* value, float width) {
        std::string Mode = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("mode");
        if(Mode == "Heading") {
        auto ret = new SettingsNode;
        };
         if(Mode == "m-pos") {
             auto ret = new SettingPosNode;
        };
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
