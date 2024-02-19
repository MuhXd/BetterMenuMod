#include <Geode/Geode.hpp>
#include <Geode/binding/GJGarageLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

class $modify(GJGarageLayer) { 
void icon(int id) {
   switch(id) {
        default: return IconType::Cube:
        case 1 return IconType::Cube:
        case 2 return IconType::Ship:
        case 3 return IconType::Ball:
        case 4 return IconType::Ufo: 
        case 5 return IconType::Wave:
        case 6 return IconType::Robot: 
        case 7 return IconType::Spider:
	    case 9 return IconType::Swing: 
        case 0 return IconType::Swing: // back or something
	    case 8 return IconType::Jetpack:
    }
    GAYMANAGER->m_playerIconType = id
    
}
int geticontonumber() {
     auto GAYMANAGER = GameManager::sharedState();
     auto id = GAYMANAGER->m_playerIconType
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
                auto Button = Build<CCSprite>::createSpriteName("GJ_sStarsIcon_001.png")
                    .intoMenuItem([](auto target) {
                        icon(geticontonumber() + 1 )
                    })
                    .pos(0, 0)
                    .parent(test);
};
}