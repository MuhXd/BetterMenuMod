#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

class $(MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        if (Mod::get()->getSettingValue<bool>("RunMainMenu")) {
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
                        ->setAxisAlignment(AxisAlignment::Even)
                    ).updateLayout();
            // i don't wanna get into custom settings please, Don't make me. 
if (Mod::get()->getSettingValue<bool>("MoveMenuPosFlip")) {
                Build(this->getChildByID("bottom-menu"))
                    .contentSize({35.750, 221.f})
                    .pos(winSize.width - 22, winSize.height - 139)
                    .scale(0.9f)
                    .layout(ColumnLayout::create()->setGap(36))
                    .updateLayout();
                Build(this->getChildByID("profile-menu"))
                    .pos(winSize.width - 49, winSize.height - 20)
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::End))
                    .scale(0.575f);
                Build(this->getChildByID("player-username"))
                    .pos(winSize.width - 40,  winSize.height - 20)
                    .anchorPoint({1.f,0.5});
} 
else {
                Build(this->getChildByID("bottom-menu"))
                    .contentSize({35.750, 218.f})
                    .pos(21, winSize.height - 149)
                    .layout(ColumnLayout::create()->setGap(36))
                    .updateLayout();
                Build(this->getChildByID("close-menu"))
                    .pos(winSize.width - 105, winSize.height - 20)
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::End))
                    .updateLayout();
                    Build(this->getChildByID("profile-menu"))
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
               .posY(80)
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
                        ->setAxisAlignment(AxisAlignment::Even)
                        ->setAutoScale(false)
                        ->setGrowCrossAxis(false)
                        ->setCrossAxisOverflow(true)
                        ).updateLayout();
            }
            // Search
            if (Mod::get()->getSettingValue<bool>("ShortcutSearch")) {
                
                this->getChildByID("more-games-menu")->setVisible(false);

                Build<CCSprite>::createSpriteName("GJ_searchBtn_001.png")
                    .scale(0.7f)
                    .intoMenuItem([](auto target) {
                        CreatorLayer::create()->onOnlineLevels(target);
                    })
                    .pos(winSize.width - 42.125, 41.500)
                    .id("search-btn"_spr)
                    .intoNewParent(CCMenu::create())
                    .pos(0, 0)
                    .parent(this)
                    .id("search-menu"_spr);
            }

            auto shortcutMenu = Build<CCMenu>::create()
                .pos(7, 6)
                .anchorPoint({0, 0})
                .parent(this)
                .layout(
                    RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start)
                    ->setGrowCrossAxis(true)
                    )
                .id("shortcuts-menu"_spr)
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

            shortcutMenu->updateLayout();
        }

        return true;
    }
};
