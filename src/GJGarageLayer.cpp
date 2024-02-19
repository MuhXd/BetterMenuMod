#include <Geode/Geode.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

class $modify(GJBallsLayer, GJGarageLayer) {
    void icon(int id) {
        auto gm = GameManager::sharedState();
        switch (id) {
        default: gm->m_playerIconType = IconType::Cube; break;
        case 1: gm->m_playerIconType = IconType::Cube; break;
        case 2: gm->m_playerIconType = IconType::Ship; break;
        case 3: gm->m_playerIconType = IconType::Ball; break;
        case 4: gm->m_playerIconType = IconType::Ufo; break;
        case 5: gm->m_playerIconType = IconType::Wave; break;
        case 6: gm->m_playerIconType = IconType::Robot; break;
        case 7: gm->m_playerIconType = IconType::Spider; break;
        case 9: gm->m_playerIconType = IconType::Swing; break;
        case 0: gm->m_playerIconType = IconType::Swing; break; // back or something
        case 8: gm->m_playerIconType = IconType::Jetpack; break;
        }
    }

    void Switch(CCObject*) {
        icon(geticontonumber() + 1);
    }

    int geticontonumber() {
        auto gm = GameManager::sharedState();
        auto id = gm->m_playerIconType;
        int iconnum = 0;
        switch (id) {
        default: iconnum = 1; break;
        case IconType::Ship: iconnum = 2; break;
        case IconType::Ball: iconnum = 3; break;
        case IconType::Ufo: iconnum = 4; break;
        case IconType::Wave: iconnum = 5; break;
        case IconType::Robot: iconnum = 6; break;
        case IconType::Spider: iconnum = 7; break;
        case IconType::Swing: iconnum = 9; break;
        case IconType::Jetpack: iconnum = 1; break;
        }
        return iconnum;
    }

    bool init() {
        if (!GJGarageLayer::init())
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto gm = GameManager::sharedState();
        auto test = Build<CCMenu>::create()
                        .pos(winSize.width - 77, 78)
                        .anchorPoint({0, 0})
                        .parent(this)
                        .contentSize({71.000f, 78.600f})
                        .layout(RowLayout::create()
                                    ->setAxisAlignment(AxisAlignment::Start)
                                    ->setGrowCrossAxis(true)
                                    ->setCrossAxisReverse(true))
                        .id("shortcuts-menu-Icon-Button"_spr)
                        .collect();

        auto spr = ButtonSprite::create("Icon Switch");
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GJBallsLayer::Switch));
        btn->setScale(0.7);
        btn->setPosition(winSize.width / 2, (winSize.height / 2) - 25);
        test->addChild(btn);

        return true;
    }
};
