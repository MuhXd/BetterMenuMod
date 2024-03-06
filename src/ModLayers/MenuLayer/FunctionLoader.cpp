#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <UIBuilder.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ModifyFiles.hpp"
static geode::Loader* get();
using namespace geode::prelude;





class $modify(MenuLayer) { 
/*
'ewww
switch that out for $modify'
mat (@mat.4) - 2024
*/
     static void onModify(auto& self) {
        self.setHookPriority("MenuLayer::init", -1); // For the icon profile bug fix
    }
    void onPlay(CCObject* sender) {
        MenuLayer::onPlay(sender);
         if (Mod::get()->getSettingValue<bool>("Merp")) {
            PlaySoundEffect("merp.mp3"_spr);
         };
    };

 void onQuit(CCObject* sender) {
        if (Mod::get()->getSettingValue<bool>("EnableExitGameButton")) {
            MenuLayer::onQuit(sender);
        }
        else 
        {
    if (Mod::get()->getSettingValue<bool>("RunMainMenu")) {   
        FLAlertLayer::create(
             "Unable to exit",
             "You have <cr>Exit Button</c> Off, Please turn it on to exit",  
            "OK"
            )->show();
        }
        else {
             MenuLayer::onQuit(sender);
        }
        };
    };

    bool init() {
        if (!MenuLayer::init())
            return false;

        if (!Mod::get()->getSettingValue<bool>("RunMainMenu")) {   
            return true;
        };
            // disables the social media and more
            UiDisable(this);
            // setup positions
            setupMenuPositions(this);
            // setup shortcuts
            setupshortcuts(this);
            // add mod buttons
            addbuttons(this);     
            // load joke settings
            JokesMain(this);
       
        return true;
    }
};
