#pragma once
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
using namespace geode::prelude;

class TextureLoaderBetterMenu : public CCLayer {
public:
    void unknown(auto target) {
       geode::log::error("Unable to Find textureLoader!");
    }
    
};
static void setupshortcuts(CCNode* layer) {
    // setup vars
    cocos2d::CCSize winSize = CCDirector::get()->getWinSize();
    // create the menu
    CCMenu* SearchMenu = Build<CCMenu>::create()
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

    if (Mod::get()->getSettingValue<bool>("ShortcutSearch") && !Loader::get()->isModLoaded("ninxout.redash")) {
        if ( layer->getChildByID("more-games-menu")) {
                    layer->getChildByID("more-games-menu")->setVisible(false);
        }
        auto Button =  Build<CCSprite>::createSpriteName("GJ_searchBtn_001.png")
            .intoMenuItem([](auto target) {
                 CreatorLayer::create()->onOnlineLevels(target);
                //reinterpret_cast<CreatorLayer*>(target)->onOnlineLevels(target);
            })
            .scale(0.75f)
            .pos(0, 0)
            .parent(SearchMenu)
            .id("search-menu"_spr);
    }
     if (Mod::get()->getSettingValue<bool>("TextureLDR-Shortcut") && (typeinfo_cast<CCMenuItemSpriteExtra*>(layer->getChildByIDRecursive("geode.texture-loader/texture-loader-button"))) && !Loader::get()->isModLoaded("ninxout.redash") ) {
        if (layer->getChildByID("more-games-menu")) {
                 layer->getChildByID("more-games-menu")->setVisible(false);
        }
        CCMenuItemSpriteExtra* geode_texture_loader_texture_loader_button =  typeinfo_cast<CCMenuItemSpriteExtra*>(layer->getChildByIDRecursive("geode.texture-loader/texture-loader-button"));
         CCSprite* TextureLDR = CCSprite::create(
            "TexturePack.png"_spr
        );
        auto TextureLDRe = CCMenuItemSpriteExtra::create(TextureLDR, layer, menu_selector(TextureLoaderBetterMenu::unknown) );
        TextureLDRe->setID("texture-loader-button"_spr);
        TextureLDRe->m_pfnSelector = geode_texture_loader_texture_loader_button->m_pfnSelector;
        geode_texture_loader_texture_loader_button->setVisible(false); // no crash by hiding it
        SearchMenu->addChild(TextureLDRe);
    }
    CCMenu* shortcutMenu = Build<CCMenu>::create()
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

        CCMenu* shortcutMenu_2;
        // change if the mod Overcharged Main Menu is loaded and hide it and create shortcutMenu a new scale and stuff
        if (Loader::get()->isModLoaded("ninxout.redash")) {
            shortcutMenu->setVisible(false);
            shortcutMenu_2 = SearchMenu;
            shortcutMenu_2->setID("shortcuts-menu-Fix"_spr);
            shortcutMenu_2->setVisible(false);
            SearchMenu = Build<CCMenu>::create()
                .pos(0, 47)
                .anchorPoint({0, 0})
                .parent(layer)
                .contentSize({20.600f, 592.001})
                .scale(0.4)
                .layout(
                    ColumnLayout::create()
                    ->setAxisAlignment(AxisAlignment::Center)
                    ->setGrowCrossAxis(false)
                    ->setCrossAxisReverse(false)
                    )
                .id("shortcuts-menu"_spr)
                .collect();

            for(auto button : CCArrayExt<CCNode*>(shortcutMenu_2->getChildren())) {
                SearchMenu->addChild(button);
            };

        } else {
            shortcutMenu_2 = Build<CCMenu>::create()
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
        }
    // My Levels
    if (Mod::get()->getSettingValue<bool>("ShortcutMyLevel") && !Loader::get()->isModLoaded("ninxout.redash")) {
        Build<CCSprite>::createSpriteName("GJ_editBtn_001.png")
            .scale(0.4f)
            .intoMenuItem([](auto target) {
                CreatorLayer::create()->onMyLevels(target);
                //reinterpret_cast<CreatorLayer*>(target)->onMyLevels(target);
            })
            .parent(shortcutMenu)
            .id("my-levels-btn"_spr);
    }

    // Saved Levels
    if (Mod::get()->getSettingValue<bool>("ShortcutSavedLevels") && !Loader::get()->isModLoaded("ninxout.redash") ) {
        Build<CCSprite>::createSpriteName("accountBtn_myLevels_001.png")
            .scale(0.7f)
            .intoMenuItem([](auto target) {
                CreatorLayer::create()->onSavedLevels(target);
                //reinterpret_cast<CreatorLayer*>(target)->onSavedLevels(target);
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


    if (Mod::get()->getSettingValue<bool>("ShortcutDaily")&& !Loader::get()->isModLoaded("ninxout.redash")) {
        auto Button = Build<CCSprite>::createSpriteName("GJ_sStarsIcon_001.png")
            .intoMenuItem([](auto target) {
                 CreatorLayer::create()->onDailyLevel(target);
                //reinterpret_cast<CreatorLayer*>(target)->onDailyLevel(target);
            })
            .pos(0, 0)
            .parent(Menu2_2);
    }
    if (Mod::get()->getSettingValue<bool>("ShortcutWeekly")&& !Loader::get()->isModLoaded("ninxout.redash")) {
    auto Button = Build<CCSprite>::createSpriteName("difficulty_06_btn_001.png")
        .intoMenuItem([](auto target) {
            CreatorLayer::create()->onWeeklyLevel(target);
            //reinterpret_cast<CreatorLayer*>(target)->onWeeklyLevel(target);
        })
        .pos(0, 0)
        .parent(Menu2_2);
    }
    Menu2_2->updateLayout();
    SearchMenu->updateLayout();
    shortcutMenu->updateLayout();
    // fixes with compact
    if (Mod::get()->getSettingValue<bool>("compact-main-menu")) {
        if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) node->setVisible(Loader::get()->isModLoaded("ninxout.redash"));
        if (CCNode* node = layer->getChildByIDRecursive("search-menu"_spr)) node->setVisible(Loader::get()->isModLoaded("ninxout.redash"));
        if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu-search"_spr)) node->setVisible(Loader::get()->isModLoaded("ninxout.redash"));
        if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) node->setVisible(Loader::get()->isModLoaded("ninxout.redash"));
        if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu"_spr)) node->setPositionX(30);
    }
}