#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <UIBuilder.hpp>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
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
    void onQuit(CCObject* sender) {
        if (Mod::get()->getSettingValue<bool>("EnableExitGameButton")) {
            MenuLayer::onQuit(sender);
        }
        else {
        FLAlertLayer::create(
             "Unable to exit",
             "You have <cr>Exit Button</c> Off Please turn it on to exit",  
                "OK"       
                )->show();
        };
    };
    bool init() {
        if (!MenuLayer::init())
            return false;

    #ifdef GEODE_IS_ANDROID
        if (Loader::get()->isModLoaded("weebify.restartbtn")) {
            if (!Mod::get()->getSettingValue<bool>("EnableExitGameButton")) {
                 this->getChildByID("close-menu")->setVisible(false);
            };
        };
    #else
        if (!Mod::get()->getSettingValue<bool>("EnableExitGameButton")) {
            this->getChildByID("close-menu")->setVisible(false);
         };
    #endif

        if (Mod::get()->getSettingValue<bool>("RunMainMenu")) {
            bool anti = false;
            auto winSize = CCDirector::get()->getWinSize();
            this->getChildByID("social-media-menu")->setVisible(false);
            if (Mod::get()->getSettingValue<bool>("HideName"))
                this->getChildByID("player-username")->setVisible(false);
            // Move the menus around
            if (Mod::get()->getSettingValue<bool>("MoveMenuPos")) {
                Build(this->getChildByID("right-side-menu"))
                    .pos(winSize.width / 2, winSize.height / 4.2)
                    .scale(1.225f)
                    .contentSize({223.749f, 69.000})
                    .layout(RowLayout::create()
                        ->setGap(2.f)
                        ->setAxisAlignment(AxisAlignment::Center)
                    ).updateLayout();
            // i don't wanna get into custom settings please, Don't make me. 
if (Mod::get()->getSettingValue<bool>("MoveMenuPosFlip")) {
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
                Build(this->getChildByID("close-menu"))
                    .pos(winSize.width - 105, winSize.height - 20)
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::End))
                    .updateLayout();
                    Build(this->getChildByID("profile-menu"))
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start))
                    .pos(48, winSize.height - 20)
                    .scale(0.575f);

                Build(this->getChildByID("player-username"))
                    .pos(39,  winSize.height - 20)
                    .anchorPoint({0.f,0.5});
}
            }
            //  😱 yea also Croozy 😱 
            else if (Mod::get()->getSettingValue<bool>("MoveMenuPosFlip"))
            {
              Build(this->getChildByID("bottom-menu")).posY(winSize.height-35.5);
               Build(this->getChildByID("main-title"))
               .posY(45)
               .scale(0.775f);
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
            if (Mod::get()->getSettingValue<bool>("MoveMenuPosFlip")) {
              Menu2_2=shortcutMenu;
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
         static_cast<cocos2d::CCLabelBMFont*>(this->getChildByID("player-username"))->setString("Streamer");
        }

        return true;
    }
};
