#include <Geode/Geode.hpp>
#include "Settings.hpp"

using namespace geode::prelude;

SettingNode* Settings::createNode(float width, auto mo) {
    return SettingsNode::create(this, width,mo);
}

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<exitheading>("exit-heading", "none","Mode");
}