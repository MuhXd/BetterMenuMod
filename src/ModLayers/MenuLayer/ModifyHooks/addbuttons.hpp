#pragma once
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
using namespace geode::prelude;
void createbuttonquicker(CCLayer* layer, const char* name, auto sprite, SEL_MenuHandler menuSel) {
    auto btn = CircleButtonSprite::createWithSprite(
        sprite,
        1.15f,
        CircleBaseColor::Green,
        CircleBaseSize::MediumAlt
    );

    auto btnee = CCMenuItemSpriteExtra::create(btn, layer, menuSel);
    btnee->setID(name);
    layer->getChildByID("bottom-menu")->addChild(btnee);
}
class CreatorLayerForBetterMenu : public CCLayer {
public:
void onSavedLevels(auto target) {
		//CreatorLayer::create()->onSavedLevels(target);
    reinterpret_cast<CreatorLayer*>(target)->onSavedLevels(target);
	}
  void onOnlineLevels(auto target) {
		 //CreatorLayer::create()->onOnlineLevels(target);
     reinterpret_cast<CreatorLayer*>(target)->onOnlineLevels(target);
	}
  void onMyLevels(auto target) {
  //  CreatorLayer::create()->onMyLevels(target);
    reinterpret_cast<CreatorLayer*>(target)->onMyLevels(target);
  }
};

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
            if (Mod::get()->getSettingValue<bool>("SavedLevelsbutton")) {
               createbuttonquicker(layer,"SavedLevels"_spr,"SavedLevels.png"_spr,menu_selector(CreatorLayerForBetterMenu::onSavedLevels));
            }
            if (Mod::get()->getSettingValue<bool>("Searchbutton")) {
               createbuttonquicker(layer,"Searchbutton"_spr,"SearchShortcut.png"_spr,menu_selector(CreatorLayerForBetterMenu::onOnlineLevels));
            }
              if (Mod::get()->getSettingValue<bool>("MyLevelbutton")) {
              createbuttonquicker(layer,"MyLevelbutton"_spr,"EditButtonSprite.png"_spr,menu_selector(CreatorLayerForBetterMenu::onMyLevels));
              // yes i rather write code then change the sprite
            if (layer->getChildByIDRecursive("MyLevelbutton"_spr)) {
               for(auto items : CCArrayExt<CCNode*>(layer->getChildByIDRecursive("MyLevelbutton"_spr)->getChildren())) {
			        if (auto icon = getChildOfType<CCSprite>(items, 0)) {
			          icon->setPositionX(25.5f);
                icon->setPositionY(28);
			          break;
    	    };
		 }
            }
            }

     layer->getChildByID("bottom-menu")->updateLayout();
return;
}