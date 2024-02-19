#include <Geode/Geode.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <UIBuilder.hpp>
#include <algorithm> // Include the algorithm header for std::find
#include <vector>    // Include the vector header for std::vector
//#include <Geode/modify/ClassName.hpp>
using namespace geode::prelude;

class $modify(GJBallsLayer,GJGarageLayer) { 

void Switch(CCObject*) {
    auto GAYMANAGER = GameManager::sharedState();
    
    // Ensure that m_playerIconType is of type IconType
    static_assert(std::is_same<decltype(GAYMANAGER->m_playerIconType), IconType>::value,
                  "m_playerIconType is not of type IconType");
    
    // Array of IconType values in the desired order
    std::vector<IconType> iconOrder = {
        IconType::Cube,
        IconType::Ship,
        IconType::Ball,
        IconType::Ufo,
        IconType::Wave,
        IconType::Robot,
        IconType::Spider,
        IconType::Swing,
        IconType::Jetpack
    };
    
    // Get the current index of m_playerIconType in iconOrder
    auto it = std::find(iconOrder.begin(), iconOrder.end(), GAYMANAGER->m_playerIconType);
    if (it != iconOrder.end()) {
        // Increment to the next icon type, looping back to the beginning if necessary
        ++it;
        if (it == iconOrder.end()) {
            it = iconOrder.begin();
        }
        // Assign the next icon type to m_playerIconType
        GAYMANAGER->m_playerIconType = *it;
    }
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
            spr, this, menu_selector($modify(GJBallsLayer,GJGarageLayer)::Switch)
        );
        btn->setScale(.7);
        btn->setPosition(winSize.width / 2, (winSize.height / 2) - 25);
         test->addChild(btn);
         return true;
};
};
