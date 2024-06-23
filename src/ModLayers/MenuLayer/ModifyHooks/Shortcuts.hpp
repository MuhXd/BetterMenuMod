#pragma once
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
using namespace geode::prelude;

static void setupshortcuts(auto layer) {
    auto winSize = CCDirector::get()->getWinSize();
    auto SearchMenu = Build<CCMenu>::create()
                .pos(winSize.width - 189, 1.500f)
                .scale(0.75f)
                .contentSize({250.000f, 104.500f})
                .anchorPoint({0, 0})
                .parent(layer)
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
        layer->getChildByID("more-games-menu")->setVisible(false);
        auto Button =  Build<CCSprite>::createSpriteName("GJ_searchBtn_001.png")
            .intoMenuItem([](auto target) {
                // CreatorLayer::create()->onOnlineLevels(target);
                reinterpret_cast<CreatorLayer*>(target)->onOnlineLevels(target);
            })
            .scale(0.75f)
            .pos(0, 0)
            .parent(SearchMenu)
            .id("search-menu"_spr);
    }
    auto shortcutMenu = Build<CCMenu>::create()
        .pos(7, 6)
        .anchorPoint({0, 0})
        .parent(layer)
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
        .parent(layer)
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
                // CreatorLayer::create()->onMyLevels(target);
                reinterpret_cast<CreatorLayer*>(target)->onMyLevels(target);
            })
            .parent(shortcutMenu)
            .id("my-levels-btn"_spr);
    }

    // Saved Levels
    if (Mod::get()->getSettingValue<bool>("ShortcutSavedLevels")) {
        Build<CCSprite>::createSpriteName("accountBtn_myLevels_001.png")
            .scale(0.7f)
            .intoMenuItem([](auto target) {
                //CreatorLayer::create()->onSavedLevels(target);
                reinterpret_cast<CreatorLayer*>(target)->onSavedLevels(target);
            })
            .parent(shortcutMenu)
            .id("saved-levels-btn"_spr);
    }
    
    auto Menu2_2 = shortcutMenu_2;
    if (v) {
        Menu2_2 = shortcutMenu;
        if (!r) {
            Menu2_2 = shortcutMenu_2;
        }
    }

    if (Mod::get()->getSettingValue<bool>("ShortcutDaily")) {
        auto Button = Build<CCSprite>::createSpriteName("GJ_sStarsIcon_001.png")
            .intoMenuItem([](auto target) {
                // CreatorLayer::create()->onDailyLevel(target);
                reinterpret_cast<CreatorLayer*>(target)->onDailyLevel(target);
            })
            .pos(0, 0)
            .parent(Menu2_2);
    }
    if (Mod::get()->getSettingValue<bool>("ShortcutWeekly")) {
    auto Button = Build<CCSprite>::createSpriteName("difficulty_06_btn_001.png")
        .intoMenuItem([](auto target) {
            //CreatorLayer::create()->onWeeklyLevel(target);
                reinterpret_cast<CreatorLayer*>(target)->onWeeklyLevel(target);
        })
        .pos(0, 0)
        .parent(Menu2_2);
    }
    Menu2_2->updateLayout();
    SearchMenu->updateLayout();
    shortcutMenu->updateLayout();
    // fixes with compact
    if (Mod::get()->getSettingValue<bool>("compact-main-menu")) {
        if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) node->setVisible(false);
        if (CCNode* node = layer->getChildByIDRecursive("search-menu"_spr)) node->setVisible(false);
        if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) node->setVisible(false);
        if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu"_spr)) node->setPositionX(30);
    }
}