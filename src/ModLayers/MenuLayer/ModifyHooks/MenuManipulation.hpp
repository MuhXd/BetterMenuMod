#pragma once
#include <Geode/Geode.hpp>
#include "../../Settings/Settings.hpp"
#include <UIBuilder.hpp>
using namespace geode::prelude;
bool v = false;
bool r = false;
bool anti = false;
// converts numbers in a square of two bool values
void GetIdsPos() {
     int menupos = Mod::get()->getSettingValue<SettingPosStruct>("MenuPos-pos").m_pos;
  switch (menupos) {
        case 1: 
        r=false;
        v=false;
        break;
        case 2:
        r=false;
        v=true;
        break;
        case 3:
        r=true;
        v=true;
        break;
        case 4: 
        r=true;
        v=false;
        break;

  }
}
static void setupMenuPositions(auto layer) {
    auto winSize = CCDirector::get()->getWinSize();
// move right/left or whatever
 if (r) {
                Build(layer->getChildByID("right-side-menu"))
                    .pos(winSize.width / 2, winSize.height / 4.2)
                    .scale(1.225f)
                    .contentSize({223.749f, 69.000})
                    .layout(RowLayout::create()
                        ->setGap(2.f)
                        ->setAxisAlignment(AxisAlignment::Center)
                    ).updateLayout();
if (v) {
                Build(layer->getChildByID("bottom-menu"))
                    .contentSize({35.750, 221.f})
                    .pos(winSize.width - 22, winSize.height - 139)
                    .scale(0.9f)
                    .layout(ColumnLayout::create()->setGap(0))
                    .updateLayout();
                auto RowLayoutFix_IconProfile = RowLayout::create();
                Build(layer->getChildByID("profile-menu"))
                    .pos(winSize.width - 39, winSize.height - 20)
                    .layout(RowLayoutFix_IconProfile->setAxisAlignment(AxisAlignment::End))
                    .contentSize({111.000f, 58.000f})
                    .scale(0.575f);   
                if (Loader::get()->isModLoaded("capeling.icon_profile_2.2")) {
                    layer->getChildByID("profile-menu")->setRotationY(180);
                    layer->getChildByID("profile-menu")->setRotationX(0);
                    RowLayoutFix_IconProfile->setAxisAlignment(AxisAlignment::Start);
                }
                    layer->getChildByID("profile-menu")->updateLayout();
                Build(layer->getChildByID("player-username"))
                    .pos(winSize.width - 40,  winSize.height - 20)
                    .anchorPoint({1.f,0.5});
} 
else {
                Build(layer->getChildByID("bottom-menu"))
                    .contentSize({35.750, 218.f})
                    .pos(21, winSize.height - 149)
                    .layout(ColumnLayout::create()->setGap(0))
                    .updateLayout();
                anti=true;
                if (layer->getChildByID("close-menu")) {
                    Build(layer->getChildByID("close-menu"))
                    .pos(winSize.width - 105, winSize.height - 20)
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::End))
                    .updateLayout();
                };
                    Build(layer->getChildByID("profile-menu"))
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start))
                    .pos(48, winSize.height - 20)
                    .scale(0.575f);

                Build(layer->getChildByID("player-username"))
                    .pos(39,  winSize.height - 20)
                    .anchorPoint({0.f,0.5});
}
            }
            else if (v)
            {
              Build(layer->getChildByID("bottom-menu")).posY(winSize.height-35.5);
              if (Mod::get()->getSettingValue<bool>("RightSide")) {
             Build(layer->getChildByID("main-title"))
               .posY(88)
               .scale(0.775f);
              }
              else {
               Build(layer->getChildByID("main-title"))
               .posY(45)
               .scale(0.775f);
              }
            }

            // move right side menu
    if (Mod::get()->getSettingValue<bool>("RightSide")) { 
               
                Build(layer->getChildByID("right-side-menu"))
                    .pos(169,  28.190f)
                    .scale(1)
                    .contentSize({144.749f, 63.825f})
                    .layout(
                        RowLayout::create()
                        ->setGap(3.f)
                        ->setAxisAlignment(AxisAlignment::Center)
                        ->setAutoScale(false)
                        ->setGrowCrossAxis(false)
                        ->setCrossAxisOverflow(true)
                        ).updateLayout();
                        if (!r) {
                            if (!v) {
                                int old = layer->getChildByID("bottom-menu")->getPositionY();
                                Build(layer->getChildByID("bottom-menu")).posY(old+30);
                            }
                        }

            }

            // tween the menu
      if (Mod::get()->getSettingValue<bool>("MenuMovement")) {
            float beforemoveposY = layer->getChildByID("bottom-menu")->getPositionY();
            float beforemoveposX = layer->getChildByID("bottom-menu")->getPositionX();
            if (r) {
                if (v) {
                    layer->getChildByID("bottom-menu")->setPositionX( (winSize.width + 45 ) );
                }  else {
                    layer->getChildByID("bottom-menu")->setPositionX( (0 - 45) );
                }   
            } else {
                if (v) {
                    layer->getChildByID("bottom-menu")->setPositionY( (winSize.height + 45) );
                }       
                else {
                    layer->getChildByID("bottom-menu")->setPositionY( (0 - 45) );
                }  
            }
             layer->getChildByID("bottom-menu")->runAction(CCEaseIn::create(CCMoveTo::create(0.5f, { beforemoveposX, beforemoveposY }), 0.35f));
        };

            return;

}