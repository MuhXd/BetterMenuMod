#pragma once
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include "../../Settings/Settings.hpp"

using namespace geode::prelude;

static CCMenuItemSpriteExtra* m_objectbettermenuhide = nullptr; // object->getParent() ??????

class pl : public CCLayer { // yes
public:
    void bettermenuhide(auto object) {
        bool enabled = m_objectbettermenuhide->getChildByIDRecursive("hiding"_spr)->isVisible();
        if (auto sc = m_objectbettermenuhide->getParent()) {
            if (auto layer = sc->getParent()) {
                layer->getChildByIDRecursive("bottom-menu")->setVisible(!enabled);
                if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) node->setVisible(!enabled);
                if (CCNode* node = layer->getChildByIDRecursive("ninxout.redash/bottom-menu-bg")) node->setVisible(!enabled);
                if (CCNode* node = layer->getChildByIDRecursive("search-menu"_spr)) node->setVisible(!enabled);
                if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu-search"_spr)) node->setVisible(!enabled);
                if (CCNode* node = layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) node->setVisible(!enabled);
                if (Loader::get()->isModLoaded("ninxout.redash")) {
                    if (CCNode* node = layer->getChildByIDRecursive("profile-menu")) node->setVisible(!enabled);
                    if (CCNode* node = layer->getChildByIDRecursive("player-username")) node->setVisible(!enabled);
                }
                
                m_objectbettermenuhide->getChildByIDRecursive("hiding"_spr)->setVisible(!enabled);
                m_objectbettermenuhide->getChildByIDRecursive("showing"_spr)->setVisible(enabled);
            }
        }
    }
};

static void compactmenu(auto layer) {
    if (!Mod::get()->getSettingValue<bool>("compact-main-menu")) {
        return;
    };
    cocos2d::CCSize winSize = CCDirector::get()->getWinSize();
    CCMenu* compact = Build<CCMenu>::create()
                    .pos(winSize.width - 90 ,  0) // 109
                    .anchorPoint({0, 0})
                    .scale(0.8f)
                    .parent(layer)
                    .contentSize({109.000f, 48.108f})
                    .layout(
                        RowLayout::create()
                        ->setAxisAlignment(AxisAlignment::Center)
                        ->setGrowCrossAxis(false)
                        ->setCrossAxisReverse(true)
                        )
                    .id("compact-bottom-menu"_spr)
                    .collect();
        
    if (Loader::get()->isModLoaded("ninxout.redash")) {
        compact->setPositionX(winSize.width / 2);
    };

    CCNode* geode = layer->getChildByIDRecursive("geode.loader/geode-button");
    CCNode* settings = layer->getChildByIDRecursive("settings-button");
    if (geode) {
        geode->removeFromParent();
        compact->addChild(geode);
    } // stops crashes
    if (settings) {
        settings->removeFromParent();
        compact->addChild(settings);
    } // stops crashes
    CircleButtonSprite* btn = CircleButtonSprite::createWithSprite(
                    "hide.png"_spr,
                    1.15f,
                    CircleBaseColor::Green,
                    CircleBaseSize::MediumAlt
                );
    CCMenuItemSpriteExtra* btnee = CCMenuItemSpriteExtra::create(btn, compact, menu_selector(pl::bettermenuhide));
    m_objectbettermenuhide = btnee;
    m_objectbettermenuhide->setID("ShowMenu"_spr);
    compact->addChild(btnee);

    for(auto items : CCArrayExt<CCNode*>(btnee->getChildren())) {
        CCNode* g = items;
        if (CCSprite* icon = getChildOfType<CCSprite>(g, 0)) {
            CCSprite* store = CCSprite::create("show.png"_spr);
            store->setScale(icon->getScale());
            store->setPositionX(icon->getPositionX());
            store->setPositionY(icon->getPositionY());
            g->addChild(store);
             if (Loader::get()->isModLoaded("ninxout.redash")) {
                store->setVisible(true);
                icon->setVisible(false);
            } else {
                store->setVisible(false);
                icon->setVisible(true);  
            }
            store->setID("hiding"_spr);
            icon->setID("showing"_spr);
            break;
        };
    } // another way would prob easier
    compact->updateLayout();

    if(CCNode* rightSideMenu = layer->getChildByID("right-side-menu")){
        Build(rightSideMenu)
            .pos(winSize.width / 2,  28.190f)
            .scale(1);

    }

    // this is just in case, i don't want more incompats or crashes with new options
    if (CCNode* searchmenu = layer->getChildByIDRecursive("shortcuts-menu-search"_spr)) searchmenu->setPositionY(winSize.height - 80);
    if (CCNode* searchmenufix = layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) searchmenufix->setPositionY(winSize.height - 122);

    if(CCNode* bottomMenu = layer->getChildByID("bottom-menu")){
        bottomMenu->setVisible(false);
        int menupos = Mod::get()->getSettingValue<SettingPosStruct>("MenuPos-pos").m_pos;
        if (menupos == 3) {
            bottomMenu->setPositionY(bottomMenu->getPositionY() - 50);
        }
    }

    if(CCNode* rightSideMenu = layer->getChildByID("right-side-menu")){
        Build(rightSideMenu)
            .contentSize({170.0f, 69.000})
            .layout(
                RowLayout::create()
                ->setGap(3.f)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setAutoScale(false)
                ->setGrowCrossAxis(false)
                ->setCrossAxisOverflow(true)
            ).updateLayout(); // quick way to make sure it is a RowLayout
        rightSideMenu->setUserObject("orientation", CCInteger::create(1));
    }

    if(CCNode* profileMenu = layer->getChildByID("profile-menu")){
        if (!Loader::get()->isModLoaded("ninxout.redash")) {
            Build(profileMenu)
                .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start))
                .pos(42, 23)
                .scale(0.575f);
            if(CCNode* playerUsername = layer->getChildByID("player-username")){
                Build(playerUsername)
                    .pos(2,  profileMenu->getPositionY()+20)
                    .anchorPoint({0.f,0.5});
            }
        }
    }

    if(CCNode* moreGamesMenu = layer->getChildByID("more-games-menu")){
        moreGamesMenu->setVisible(false);
    }

    if(CCNode* bottomMenu = layer->getChildByID("bottom-menu")){
        if (!Mod::get()->getSettingValue<bool>("RightSide")) { 
            int old = bottomMenu->getPositionY();
            Build(bottomMenu).posY(old+30);
        }
    }
    
    return;
}
static void compactmainmenuFix(auto layer) {
    if (Mod::get()->getSettingValue<bool>("compact-main-menu")) {
        layer->getChildByIDRecursive("bottom-menu")->setVisible(Loader::get()->isModLoaded("ninxout.redash"));
        if (Loader::get()->isModLoaded("ninxout.redash")) {
            if (CCNode* node = layer->getChildByIDRecursive("right-side-menu")){ node->setPositionX(143); node->setContentSize({373.667,node->getContentSize().height}); };
            if (CCNode* node = layer->getChildByIDRecursive("paged-right-side-menu")) { 
                node->setUserObject("real-scale",CCFloat::create(0.575));
                node->setScale(0.575);
            }
        }
    }
}

