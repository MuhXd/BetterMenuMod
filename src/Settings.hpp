#pragma once
#include <Geode/loader/SettingNode.hpp>
using namespace geode::prelude;

class Settings;

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
 bool init(Settings* value, auto mode,auto txt, float width) {
        if (!SettingNode::init(value))
            return false;
        this->setContentSize({ width, 35.f });
        auto menu = CCMenu::create();
        auto label = CCLabelBMFont::create(txt, "bigFont.fnt");
        label->setScale(.33F);
        label->setPositionX(-93);
        menu->setPosition(width / 2, 18.f);
        menu->addChild(label);
        this->addChild(menu);
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
    static SettingsNode* create(Settings* value,auto mode,auto txt, float width) {
        auto ret = new SettingClickNode;
        if (ret && ret->init(value,mode,txt, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};