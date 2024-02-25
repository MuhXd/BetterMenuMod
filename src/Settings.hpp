#pragma once
#include <Geode/loader/SettingNode.hpp>
#include <Geode/Geode.hpp>
#include <chrono>
#include <thread>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
using namespace geode::prelude;
int Heading = 0
class Settings;
void HeadingUno(auto thisui, float width) {
         auto menu = CCMenu::create();
        auto label = CCLabelBMFont::create("Remove / Disable", "bigFont.fnt");
        label->setScale(0.750);
        label->setPositionX(0);
        menu->setPosition(width / 2, 18.f);
        menu->addChild(label);
        thisui->addChild(menu);
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
        Heading=Heading+1
        if (Heading == 1) {
        HeadingUno(this,width);
        }
        else {
            auto menu = CCMenu::create();
        auto label = CCLabelBMFont::create("Gay sex", "bigFont.fnt");
        label->setScale(0.750);
        label->setPositionX(0);
        menu->setPosition(width / 2, 18.f);
        menu->addChild(label);
        thisui->addChild(menu);
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
        auto ret = new SettingsNode;
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
