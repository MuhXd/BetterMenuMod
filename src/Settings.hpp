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


void createHeading(auto width,auto thisL, auto value) {
        if(value) {
            std::string name = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("name");
        }
        else {
            auto head = "ERROR LOADING";
        }
        auto menu = CCMenu::create();
        auto label = CCLabelBMFont::create(head.c_str(), "bigFont.fnt");
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
            std::string Mode = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("mode").c_str();
        if(Mode == "Heading") {
            createHeading(width,this,value);
        }
       
        
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
        auto ret = new SettingsNode;
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

