#pragma once
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include "../Settings/Settings.hpp"
#include "SwelvyBG.hpp"
using namespace geode::prelude;

static void SwelvyBGInsert(auto layer, std::string Layer) {
    if (Layer == "MenuLayer") {
    if (!Mod::get()->getSettingValue<bool>("MenuLayer-Geode-SwelvyBG")) {   
        return;
    }
    SwelvyBG* bg = SwelvyBG::create();
    bg->setZOrder(-1);
    bg->setVisible(false);
    layer->addChild(bg); 
    
    if (auto l = layer->getChildByIDRecursive("main-menu-bg")) {
        l->setZOrder(-2);
    };

    return;
    }
    if (Layer == "LoadingLayer") {
         if (!Mod::get()->getSettingValue<bool>("LoadingLayer-Geode-SwelvyBG")) {   
        return;
    }
    SwelvyBG* bg = SwelvyBG::create();
    bg->setZOrder(-1);
    layer->addChild(bg); 
    
    if (auto l = layer->getChildByIDRecursive("bg-texture")) {
        l->setZOrder(-2);
    };

    return;
    }
    
    return;
}