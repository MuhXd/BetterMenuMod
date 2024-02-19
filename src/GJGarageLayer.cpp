#include <Geode/Geode.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <UIBuilder.hpp>
//#include <Geode/modify/ClassName.hpp>
using namespace geode::prelude;

class $modify(GJBallsLayer,GJGarageLayer) { 

void Switch(CCObject*) {
    auto GAYMANAGER = GameManager::sharedState();
    int a = GAYMANAGER->m_playerIconType;
    GAYMANAGER->m_playerIconType = ( a + 1);
}

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
            spr, this, menu_selector(GJBallsLayer::Switch)
        );
        btn->setScale(.7);
        btn->setPosition(winSize.width / 2, (winSize.height / 2) - 25);
         test->addChild(btn);
         return true;
};
};