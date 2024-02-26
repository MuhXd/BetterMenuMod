#include <Geode/Geode.hpp>
#include "Settings.hpp"

using namespace geode::prelude;



SettingNode* Settings::createNode(float width) {
    return SettingsNode::create(this, width);
}
SettingNode* SettingPosValue::createNode(float width) {
    return SettingPosNode::create(this, width);
}

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<Settings>("exit-heading", "none");
    Mod::get()->addCustomSetting<Settings>("Shortcuts-heading", "none");
    Mod::get()->addCustomSetting<Settings>("MenuPos-heading", "none");
     Mod::get()->addCustomSetting<SettingPosValue>("MenuPos-pos", 4);
}