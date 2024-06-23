#pragma once
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

static void UiDisable(auto layer) {
    if (!Mod::get()->getSettingValue<bool>("EnableExitGameButton")) {
        if(layer->getChildByID("close-menu")) {
            layer->getChildByID("close-menu")->setVisible(false);
            layer->getChildByID("close-menu")->setScale(0);
        }
    }
    layer->getChildByID("social-media-menu")->setVisible(false);
    layer->getChildByID("social-media-menu")->setScale(0);
    if (Mod::get()->getSettingValue<bool>("HideName")) {
        layer->getChildByID("player-username")->setVisible(false);
        layer->getChildByID("player-username")->setScale(0);
    }
    if (Mod::get()->getSettingValue<bool>("replacename")) {
        if (!Loader::get()->isModLoaded("coopeeo.customname")) {
            static_cast<cocos2d::CCLabelBMFont*>(layer->getChildByID("player-username"))->setString("Streamer");
        }
    };
}