#include <Geode/Geode.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

class $modify(GJBallsLayer, GJGarageLayer) {

    void Switch(CCObject*) {
         auto gm = GameManager::sharedState();
        if gm->m_playerIconType == IconType::Cube {
            gm->m_playerIconType = IconType::Ship;
            return;
        }
         if gm->m_playerIconType == IconType::Ship {
            gm->m_playerIconType = IconType::Ball;
            return;
        }
         if gm->m_playerIconType == IconType::Ball {
            gm->m_playerIconType = IconType::Ufo;
            return;
        }
         if gm->m_playerIconType == IconType::Ufo {
            gm->m_playerIconType = IconType::Wave;
            return;
        }
         if gm->m_playerIconType == IconType::Wave {
            gm->m_playerIconType = IconType::Robot;
            return;
        }
         if gm->m_playerIconType == IconType::Robot {
            gm->m_playerIconType = IconType::Spider;
            return;
        }
         if gm->m_playerIconType == IconType::Spider {
            gm->m_playerIconType = IconType::Swing;
            return;
        }
         if gm->m_playerIconType == IconType::Swing {
            gm->m_playerIconType = IconType::Jetpack;
            return;
        }
         if gm->m_playerIconType == IconType::Jetpack {
            gm->m_playerIconType = IconType::Cube;
            return;
        }
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
