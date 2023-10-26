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
                Build(this->getChildByID("close-menu"))
                    .pos(winSize.width - 105, winSize.height - 20)
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::End))
                    .updateLayout();

                Build(this->getChildByID("right-side-menu"))
                    .pos(winSize.width / 2, winSize.height / 4.2)
                    .scale(1.225)
                    .contentSize({223.749, 69.000})
                    .layout(RowLayout::create()
                        ->setGap(2.f)
                        ->setAxisAlignment(AxisAlignment::Even)
                    ).updateLayout();

                Build(this->getChildByID("bottom-menu"))
                    .scale(0.95)
                    .contentSize({35.f, 198.f})
                    .pos(21, winSize.height - 134)
                    .layout(ColumnLayout::create()->setGap(3.5))
                    .updateLayout();

                Build(this->getChildByID("profile-menu"))
                    .pos(48, winSize.height - 20)
                    .scale(0.575);

                Build(this->getChildByID("player-username"))
                    .pos(92,  winSize.height - 20)
                    .scale(1);
            }

            // Search
            if (Mod::get()->getSettingValue<bool>("ShortcutSearch")) {
                this->getChildByID("more-games-menu")->setVisible(false);

                Build<CCSprite>::createSpriteName("GJ_searchBtn_001.png")
                    .scale(0.7)
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
                .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start))
                .id("shortcuts-menu"_spr)
                .collect();

            // My Levels
            if (Mod::get()->getSettingValue<bool>("ShortcutMyLevel")) {
                Build<CCSprite>::createSpriteName("GJ_editBtn_001.png")
                    .scale(0.4)
                    .intoMenuItem([](auto target) {
                        CreatorLayer::create()->onMyLevels(target);
                    })
                    .parent(shortcutMenu)
                    .id("my-levels-btn"_spr);
            }

            // Saved Levels
            if (Mod::get()->getSettingValue<bool>("ShortcutSavedLevels")) {
                Build<CCSprite>::createSpriteName("accountBtn_myLevels_001.png")
                    .scale(0.7)
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
