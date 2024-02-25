#include <Geode/Geode.hpp>
#include "Settings.hpp"

using namespace geode::prelude;

SettingNode* Settings::createNode(float width) {
    return SettingsNode::create(this, width);
}

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<Settings>("exit-heading", "heading","Test");
}