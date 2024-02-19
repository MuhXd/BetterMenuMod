#include <Geode/Geode.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;
auto 
class $modify(GJBallsLayer,GJGarageLayer) { 
void icon(int id) {
   switch(id) {
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
    GJBallsLayer::icon(GJBallsLayer::geticontonumber() + 1);
}
int geticontonumber() {
     auto GAYMANAGER = GameManager::sharedState();
     auto id = GAYMANAGER->m_playerIconType;
    switch(id) {
        default: return 1
        case IconType::Ship: return 2
        case IconType::Ball: return 3
        case IconType::Ufo: return 4
        case IconType::Wave: return 5
        case IconType::Robot: return 6
        case IconType::Spider: return 7
	    case IconType::Swing: return 9
	    case IconType::Jetpack: return 8
    }
};
bool init() {
        if (!GJGarageLayer::init())
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto GAYMANAGER = GameManager::sharedState();
       auto test = Build<CCMenu>::create()
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
                .id("shortcuts-menu-Icon-Button"_spr)
                .collect();
              auto spr = ButtonSprite::create("Icon Switch");
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(GJGarageLayer::Switch)
        );
        btn->setScale(.7);
        btn->setPosition(winSize.width / 2, (winSize.height / 2) - 25);
         test->addChild(btn);
};
};