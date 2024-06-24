#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <UIBuilder.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ModifyFiles.hpp"
#ifdef GEODE_IS_WINDOWS
#include <cstdlib>
#include <windows.h>
#include <shellapi.h>
#endif

// english or german
// english : somewhere go find it
// german : somewhere go find it

static geode::Loader* get();
using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
bool RunAsAdmin(HWND hwnd, LPCSTR lpFile, LPCSTR lpParameters) {
    SHELLEXECUTEINFOA sei = { sizeof(sei) };
    sei.lpVerb = "runas";
    sei.lpFile = lpFile;
    sei.lpParameters = lpParameters;
    sei.hwnd = hwnd;
    sei.nShow = SW_NORMAL;

    if (!ShellExecuteExA(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED)
            // The user refused the elevation.
            return false;
        // More error handling here...
    }
    return true;
}
#endif


// separate hook to run tween after pages api updates the menu

class $modify(MenuLayer) { 

    static void onModify(auto& self) {
        (void)self.setHookPriority("MenuLayer::init", INT_MIN/2-1); // For the pages bugfix
    }

    bool init() {
        if (!MenuLayer::init())
            return false;
            
        if (!Mod::get()->getSettingValue<bool>("RunMainMenu")) {   
            return true;
        };

        runTween(this);
        compactmainmenuFix(this);
        return true;
    }
};

// German
// Der erste, der sich bewegt, ist schwul

class $modify(MenuLayer) { 
/*
'ewww
switch that out for $modify'
mat (@mat.4) - 2024
*/
    static void onModify(auto& self) {
        (void)self.setHookPriority("MenuLayer::init", -1); // For the icon profile bug fix and pages bugfix
    }
    void onPlay(CCObject* sender) {
        MenuLayer::onPlay(sender);
         if (Mod::get()->getSettingValue<bool>("Merp")) {
            PlaySoundEffect("merp.mp3"_spr);
         };
    };

 void onQuit(CCObject* sender) {
        if (Mod::get()->getSettingValue<bool>("EnableExitGameButton")) {
                #ifdef GEODE_IS_WINDOWS
            if (Mod::get()->getSettingValue<bool>("TaskKill-Process")) {   
                geode::createQuickPopup(
				"Quit Game",
				"Are you sure you want to <cr>Task Kill</c> the game?",
				"Cancel", "Yes",
				[this, sender](auto, bool btn2) {
					if (btn2) {
						RunAsAdmin(NULL, "cmd.exe", "/c TASKKILL /IM GeometryDash.exe /F");
					}
                }
			);
            return;
		};
            #endif

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
                 #ifdef GEODE_IS_WINDOWS
            if (Mod::get()->getSettingValue<bool>("TaskKill-Process")) {   
                geode::createQuickPopup(
				"Quit Game",
				"Are you sure you want to <cr>Task Kill</c> the game?",
				"Cancel", "Yes",
				[this, sender](auto, bool btn2) {
					if (btn2) {
						RunAsAdmin(NULL, "cmd.exe", "/c TASKKILL /IM GeometryDash.exe /F");
					}
                }
			);
            return;
		};
            #endif

             MenuLayer::onQuit(sender);
        }
        };
    };

// English
// First one who moves is gay

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
             // compactify all
            compactmenu(this);
            // add mod buttons
            addbuttons(this);     
            // load joke settings
            JokesMain(this);
       
        return true;
    }
};