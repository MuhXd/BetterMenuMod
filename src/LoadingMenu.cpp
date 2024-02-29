#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <UIBuilder.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <chrono>
#include <thread>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include "Settings.hpp"
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
static geode::Loader* get();
using namespace geode::prelude;
bool r = false;
bool v = false;
void PlaySoundEffect(auto effect){
	if (Mod::get()->getSettingValue<bool>("SFX")) {
		FMODAudioEngine::sharedEngine()->playEffect(effect);
	};
}
void Script(int P, auto object, float speed, float Pos) {
    if (P == 1) {
        auto add = object->getPositionY() - Pos;
        add = add / speed;
        add = add * 0.001;
         while (!object->getPositionY() - Pos == 0) {
            auto posobject = object->getPositionY();
            object->setPositionY(posobject+add);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
};

void BottomMenu(auto layer, auto d) {
    Script(1,layer,2);
};


class FakeLayer : public CCLayer {
public:
void onGeode(CCObject*) {
		PlaySoundEffect("UwU_1.mp3"_spr);
		geode::openModsList();
	}
};
void GetIdsPos() {
     int menupos = Mod::get()->getSettingValue<SettingPosStruct>("MenuPos-pos").m_pos;
  switch (menupos) {
        case 1: 
        r=false;
        v=false;
        break;
        case 2:
        r=false;
        v=true;
        break;
        case 3:
        r=true;
        v=true;
        break;
        case 4: 
        r=true;
        v=false;
        break;

  }
}
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

        if (Mod::get()->getSettingValue<bool>("RunMainMenu")) {   

            if (!Mod::get()->getSettingValue<bool>("EnableExitGameButton")) {
                if(this->getChildByID("close-menu")) {
                    this->getChildByID("close-menu")->setVisible(false);
                }
            }
            
            bool anti = false;
            auto winSize = CCDirector::get()->getWinSize();
            this->getChildByID("social-media-menu")->setVisible(false);
            if (Mod::get()->getSettingValue<bool>("HideName"))
                this->getChildByID("player-username")->setVisible(false);
            // Move the menus around
            GetIdsPos(); // loads the r and v pos values
            if (r) {
                Build(this->getChildByID("right-side-menu"))
                    .pos(winSize.width / 2, winSize.height / 4.2)
                    .scale(1.225f)
                    .contentSize({223.749f, 69.000})
                    .layout(RowLayout::create()
                        ->setGap(2.f)
                        ->setAxisAlignment(AxisAlignment::Center)
                    ).updateLayout();
if (v) {
                Build(this->getChildByID("bottom-menu"))
                    .contentSize({35.750, 221.f})
                    .pos(winSize.width - 22, winSize.height - 139)
                    .scale(0.9f)
                    .layout(ColumnLayout::create()->setGap(0))
                    .updateLayout();
                auto RowLayoutFix_IconProfile = RowLayout::create();
                Build(this->getChildByID("profile-menu"))
                    .pos(winSize.width - 39, winSize.height - 20)
                    .layout(RowLayoutFix_IconProfile->setAxisAlignment(AxisAlignment::End))
                    .contentSize({111.000f, 58.000f})
                    .scale(0.575f);   
                if (Loader::get()->isModLoaded("capeling.icon_profile_2.2")) {
                    this->getChildByID("profile-menu")->setRotationY(180);
                    this->getChildByID("profile-menu")->setRotationX(0);
                    RowLayoutFix_IconProfile->setAxisAlignment(AxisAlignment::Start);
                }
                    this->getChildByID("profile-menu")->updateLayout();
                Build(this->getChildByID("player-username"))
                    .pos(winSize.width - 40,  winSize.height - 20)
                    .anchorPoint({1.f,0.5});
} 
else {
                Build(this->getChildByID("bottom-menu"))
                    .contentSize({35.750, 218.f})
                    .pos(21, winSize.height - 149)
                    .layout(ColumnLayout::create()->setGap(0))
                    .updateLayout();
                anti=true;
                if (this->getChildByID("close-menu")) {
                    Build(this->getChildByID("close-menu"))
                    .pos(winSize.width - 105, winSize.height - 20)
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::End))
                    .updateLayout();
                };
                    Build(this->getChildByID("profile-menu"))
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start))
                    .pos(48, winSize.height - 20)
                    .scale(0.575f);

                Build(this->getChildByID("player-username"))
                    .pos(39,  winSize.height - 20)
                    .anchorPoint({0.f,0.5});
}
            }
            else if (v)
            {
              Build(this->getChildByID("bottom-menu")).posY(winSize.height-35.5);
              if (Mod::get()->getSettingValue<bool>("RightSide")) {
             Build(this->getChildByID("main-title"))
               .posY(88)
               .scale(0.775f);
              }
              else {
               Build(this->getChildByID("main-title"))
               .posY(45)
               .scale(0.775f);
              }
            }
            // Moves right side menu down bescause Croozy wanted it to.
             if (Mod::get()->getSettingValue<bool>("RightSide")) { 
               
                Build(this->getChildByID("right-side-menu"))
                    .pos(169,  28.190f)
                    .scale(1)
                    .contentSize({144.749f, 63.825f})
                    .layout(
                        RowLayout::create()
                        ->setGap(3.f)
                        ->setAxisAlignment(AxisAlignment::Center)
                        ->setAutoScale(false)
                        ->setGrowCrossAxis(false)
                        ->setCrossAxisOverflow(true)
                        ).updateLayout();
                        if (!r) {
                            if (!v) {
                                int old = this->getChildByID("bottom-menu")->getPositionY();
                                Build(this->getChildByID("bottom-menu")).posY(old+30);
                            }
                        }

            }
            // Creates the Menu
            auto SearchMenu = Build<CCMenu>::create()
                .pos(winSize.width - 189, 1.500f)
                .scale(0.75f)
                .contentSize({250.000f, 104.500f})
                .anchorPoint({0, 0})
                .parent(this)
                .layout(
                    RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::End)
                    ->setGrowCrossAxis(true)
                    ->setAxisReverse(true)
                    )
                .id("shortcuts-menu-search"_spr)
                .collect();
                bool Size = false;
            if (Mod::get()->getSettingValue<bool>("ShortcutSearch")) {
                this->getChildByID("more-games-menu")->setVisible(false);
               auto Button =  Build<CCSprite>::createSpriteName("GJ_searchBtn_001.png")
                    .intoMenuItem([](auto target) {
                        CreatorLayer::create()->onOnlineLevels(target);
                    })
                    .scale(0.75f)
                    .pos(0, 0)
                    .parent(SearchMenu)
                    .id("search-menu"_spr);
            }
            auto shortcutMenu = Build<CCMenu>::create()
                .pos(7, 6)
                .anchorPoint({0, 0})
                .parent(this)
                .contentSize({71.000f, 78.600f})
                .layout(
                    RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::Center)
                    ->setGrowCrossAxis(true)
                    ->setCrossAxisReverse(true)
                    )
                .id("shortcuts-menu"_spr)
                .collect();

        auto shortcutMenu_2 = Build<CCMenu>::create()
                .pos(winSize.width - 77, 78)
                .anchorPoint({0, 0})
                .parent(this)
                .contentSize({71.000f, 78.600f})
                .layout(
                    RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start)
                    ->setGrowCrossAxis(true)
                    ->setCrossAxisReverse(true)
                    )
                .id("shortcuts-menu-Fix"_spr)
                .collect();
    
            // My Levels
            if (Mod::get()->getSettingValue<bool>("ShortcutMyLevel")) {
                Build<CCSprite>::createSpriteName("GJ_editBtn_001.png")
                    .scale(0.4f)
                    .intoMenuItem([](auto target) {
                        CreatorLayer::create()->onMyLevels(target);
                    })
                    .parent(shortcutMenu)
                    .id("my-levels-btn"_spr);
            }
    
            // Saved Levels
            if (Mod::get()->getSettingValue<bool>("ShortcutSavedLevels")) {
                Build<CCSprite>::createSpriteName("accountBtn_myLevels_001.png")
                    .scale(0.7f)
                    .intoMenuItem([](auto target) {
                        CreatorLayer::create()->onSavedLevels(target);
                    })
                    .parent(shortcutMenu)
                    .id("saved-levels-btn"_spr);
            }
            
            auto Menu2_2=shortcutMenu_2;
            if (v) {
              Menu2_2=shortcutMenu;
               if (!r) {
                    Menu2_2=shortcutMenu_2;
               }
            }

            if (Mod::get()->getSettingValue<bool>("ShortcutDaily")) {
                auto Button = Build<CCSprite>::createSpriteName("GJ_sStarsIcon_001.png")
                    .intoMenuItem([](auto target) {
                        CreatorLayer::create()->onDailyLevel(target);
                    })
                    .pos(0, 0)
                    .parent(Menu2_2);
            }
             if (Mod::get()->getSettingValue<bool>("ShortcutWeekly")) {
                auto Button = Build<CCSprite>::createSpriteName("difficulty_06_btn_001.png")
                    .intoMenuItem([](auto target) {
                        CreatorLayer::create()->onWeeklyLevel(target);
                    })
                    .pos(0, 0)
                    .parent(Menu2_2);
            }
            Menu2_2->updateLayout();
            shortcutMenu->updateLayout();
            SearchMenu->updateLayout();
        }
        if (Mod::get()->getSettingValue<bool>("replacename")) {
               if (!Loader::get()->isModLoaded("coopeeo.customname")) {
                    static_cast<cocos2d::CCLabelBMFont*>(this->getChildByID("player-username"))->setString("Streamer");
               }
        };

        if (Mod::get()->getSettingValue<bool>("Uwugeode")) {
        auto geodeButton = dynamic_cast<CCMenuItemSpriteExtra*>(
		getChildByIDRecursive("geode.loader/geode-button")
	    );
		geodeButton->m_pfnSelector = menu_selector(FakeLayer::onGeode);
		 for(auto items : CCArrayExt<CCNode*>(this->getChildByIDRecursive("geode.loader/geode-button")->getChildren())) {
			auto g=items;
			if (auto icon = getChildOfType<CCSprite>(g, 0)) {
        	auto Spr = CCSprite::create("gwode.png"_spr);
			Spr->setScale(0.2);
			Spr->setPositionX(icon->getPositionX());
			Spr->setPositionY(icon->getPositionY());
			g->addChild(Spr);
			icon->setVisible(false);
			break;
    	    };
		 }
         };

        if (Mod::get()->getSettingValue<bool>("Merp")) {
            auto title = this->getChildByID("main-title");
            auto titlePos = title->getPosition();
            auto Spr = CCSprite::create("MERP.png"_spr);
            Spr->setPositionX(titlePos.x);
            Spr->setPositionY(titlePos.y);
            this->removeChildByID("main-title");
            Spr->setID("main-title");
            this->addChild(Spr);
        };

        if (Mod::get()->getSettingValue<bool>("MenuMovement")) {
           //  
            float beforemoveposY = this->getChildByID("bottom-menu")->getPositionY();
            float beforemoveposX = this->getChildByID("bottom-menu")->getPositionX();
            if (r) {
                if (v) {
                    this->getChildByID("bottom-menu")->setPositionX( (beforemoveposX + 100) );
                }  else {
                    this->getChildByID("bottom-menu")->setPositionX( (beforemoveposX - 100) );
                }   
            } else {
                if (v) {
                    this->getChildByID("bottom-menu")->setPositionY( (beforemoveposY + 100) );
                }       
                else {
                    this->getChildByID("bottom-menu")->setPositionY( (beforemoveposY - 100) );
                }  
            }
            #ifdef GEODE_IS_WINDOWS || GEODE_IS_ANDROID
               this->getChildByID("bottom-menu")->runAction(CCEaseInOut::create(CCMoveTo::create(1.0f, { beforemoveposX, beforemoveposY }), 1.0f));
            #else
                this->getChildByID("bottom-menu")->runAction(CCMoveTo::create(1.0f,{ beforemoveposX, beforemoveposY }));
           #endif
        };

        return true;
    }
};