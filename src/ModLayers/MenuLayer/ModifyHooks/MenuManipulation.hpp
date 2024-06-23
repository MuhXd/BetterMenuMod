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
            r = false;
            v = false;
            break;
        case 2:
            r = false;
            v = true;
            break;
        case 3:
            r = true;
            v = true;
            break;
        case 4: 
            r = true;
            v = false;
            break;
    }
}

static void setupMenuPositions(auto layer) {
    auto winSize = CCDirector::get()->getWinSize();
    GetIdsPos();

    // move right/left or whatever
    if (r) {
        if(CCNode* rightSideMenu = layer->getChildByID("right-side-menu")){
            Build(rightSideMenu)
                .pos(winSize.width / 2, winSize.height / 4.2)
                .scale(1.225f)
                .contentSize({223.749f, 69.000})
                .layout(RowLayout::create()
                    ->setGap(2.f)
                    ->setAxisAlignment(AxisAlignment::Center)
                ).updateLayout();
            rightSideMenu->setUserObject("orientation", CCInteger::create(1));
            rightSideMenu->setUserObject("gap", CCFloat::create(20));
        }

        if (v) {
            if(CCNode* bottomMenu = layer->getChildByID("bottom-menu")){
                Build(bottomMenu)
                    .contentSize({35.750, 221.f})
                    .pos(winSize.width - 22, winSize.height - 139)
                    .scale(0.9f)
                    .layout(ColumnLayout::create()->setGap(0))
                    .updateLayout();
                bottomMenu->setUserObject("orientation", CCInteger::create(0));
            }

            if(CCNode* profileMenu = layer->getChildByID("profile-menu")){
                auto RowLayoutFix_IconProfile = RowLayout::create();
                Build(profileMenu)
                    .pos(winSize.width - 39, winSize.height - 20)
                    .layout(RowLayoutFix_IconProfile->setAxisAlignment(AxisAlignment::End))
                    .contentSize({111.000f, 58.000f})
                    .scale(0.575f);   
                if (Loader::get()->isModLoaded("capeling.icon_profile_2.2")) {
                    profileMenu->setRotationY(180);
                    profileMenu->setRotationX(0);
                    RowLayoutFix_IconProfile->setAxisAlignment(AxisAlignment::Start);
                }
                profileMenu->updateLayout();
            }

            if(CCNode* playerUsername = layer->getChildByID("player-username")){
                Build(playerUsername)
                    .pos(winSize.width - 40,  winSize.height - 20)
                    .anchorPoint({1.f,0.5});
            }
            
        } 
        else {

            if(CCNode* bottomMenu = layer->getChildByID("bottom-menu")){
                Build(bottomMenu)
                    .contentSize({35.750, 218.f})
                    .pos(21, winSize.height - 149)
                    .layout(ColumnLayout::create()->setGap(0))
                    .updateLayout();
                bottomMenu->setUserObject("orientation", CCInteger::create(0));
            }
            anti=true;

            if(CCNode* closeMenu = layer->getChildByID("close-menu")){
                Build(closeMenu)
                    .pos(winSize.width - 105, winSize.height - 20)
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::End))
                    .updateLayout();
            }

            if(CCNode* profileMenu = layer->getChildByID("profile-menu")){
                Build(profileMenu)
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start))
                    .pos(48, winSize.height - 20)
                    .scale(0.575f);
            }

            if(CCNode* playerUsername = layer->getChildByID("player-username")){
                Build(playerUsername)
                    .pos(39,  winSize.height - 20)
                    .anchorPoint({0.f,0.5});
            }
           
        }
    }
    else if (v) {
        if(CCNode* bottomMenu = layer->getChildByID("bottom-menu")){
            Build(bottomMenu)
                .posY(winSize.height-35.5);
        }

        if(CCNode* mainTitle = layer->getChildByID("main-title")){
            if (Mod::get()->getSettingValue<bool>("RightSide")) {
                Build(mainTitle)
                .posY(88)
                .scale(0.775f);
            }
            else {
                Build(mainTitle)
                .posY(45)
                .scale(0.775f);
            }
        }
    }
    // move right side menu
    if (Mod::get()->getSettingValue<bool>("RightSide")) { 

        if(CCNode* rightSideMenu = layer->getChildByID("right-side-menu")){
            Build(rightSideMenu)
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
                            if(CCNode* bottomMenu = layer->getChildByID("bottom-menu")){
                                int old = bottomMenu->getPositionY();
                                Build(bottomMenu)
                                    .posY(old+30);
                            }
                        }
                    }
            rightSideMenu->setUserObject("orientation", CCInteger::create(1));
            rightSideMenu->setUserObject("gap", CCFloat::create(20));
        }
    }
}

static void runTween(auto layer) {
    // tween the menu
    auto winSize = CCDirector::get()->getWinSize();

    if (!Mod::get()->getSettingValue<bool>("compact-main-menu")) {
        if (Mod::get()->getSettingValue<bool>("MenuMovement")) {

            if(CCNode* bottomMenu = layer->getChildByID("bottom-menu")) {
                float beforemoveposY = bottomMenu->getPositionY();
                float beforemoveposX = bottomMenu->getPositionX();

                if (r) {
                    if (v) {
                        bottomMenu->setPositionX( (winSize.width + 45 ) );
                    }  else {
                        bottomMenu->setPositionX( (0 - 45) );
                    }   
                } else {
                    if (v) {
                        bottomMenu->setPositionY( (winSize.height + 45) );
                    }       
                    else {
                        bottomMenu->setPositionY( (0 - 45) );
                    }  
                }
                bottomMenu->runAction(CCEaseIn::create(CCMoveTo::create(0.5f, { beforemoveposX, beforemoveposY }), 0.35f));
            }
        };
    }
}