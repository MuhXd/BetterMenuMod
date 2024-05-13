#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
using namespace geode::prelude;
static void addbuttons(auto layer) {
if (Mod::get()->getSettingValue<bool>("NoNewGroundsButton")) {
                if (auto newgrounds = layer->getChildByID("bottom-menu")->getChildByIDRecursive("newgrounds-button"))
                { 
                    newgrounds->removeFromParent();
                };
              }
              if (Mod::get()->getSettingValue<bool>("MoreGamesBottomMenu")) {
         
            auto btn = CircleButtonSprite::createWithSprite(
                "epic_text.png"_spr,
                1.15f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );
            
		        auto btnee = CCMenuItemSpriteExtra::create(btn, layer, menu_selector(MenuLayer::onMoreGames));
            btnee->setID("MoreGames"_spr);
            layer->getChildByID("bottom-menu")->addChild(btnee);
              };

     layer->getChildByID("bottom-menu")->updateLayout();
return;
}