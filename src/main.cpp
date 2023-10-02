#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/EndLevelLayer.hpp>
#include <Geode/cocos/base_nodes/Layout.hpp> 
// TOP RIGHT CLOSE
// to LEFT PROFILE FUNNI
using namespace geode::prelude;
auto Mainbooter = true;
auto FirstBoot = false;
struct $modify(MenuLayer) {

    bool init() {
        if (!MenuLayer::init()) {
            return false;
		};
	
		auto close = this->getChildByID("close-menu");
close->setLayout(
    RowLayout::create()
        ->setGap(5.f)
        ->setAutoScale(true)
        ->setAxis(Axis::Row)
		->setAxisAlignment(AxisAlignment::End)
		->setCrossAxisAlignment(AxisAlignment::Center)
		->setCrossAxisLineAlignment(AxisAlignment::Center)
);
 close->setPosition(466.9,302);

auto RightSideMenu = this->getChildByID("right-side-menu");
RightSideMenu->setLayout(
    RowLayout::create()
        ->setGap(5.f)
        ->setAutoScale(true)
        ->setAxis(Axis::Row)
		->setAxisAlignment(AxisAlignment::Center)
		->setCrossAxisAlignment(AxisAlignment::Center)
		->setCrossAxisLineAlignment(AxisAlignment::Center)
);
 RightSideMenu->setPosition(282,70);
 RightSideMenu->setScale(1.225);
 RightSideMenu->setContentSize({ 223.749, 69.000 });
         auto winSize = CCDirector::get()->getWinSize();
		/*
        auto title = this->getChildByID("main-title");
		title->setPosition(0, 0);
		  title->setScale(0);
		  Title Remover LOL
		*/

	 auto bottommenu = this->getChildByID("bottom-menu");
		 bottommenu->setScale(0.95);

	bottommenu->setLayout(
    RowLayout::create()
        ->setGap(5.f)
        ->setAutoScale(true)
        ->setAxis(Axis::Column)
		->setAxisAlignment(AxisAlignment::Center)
		->setCrossAxisAlignment(AxisAlignment::Center)
		->setCrossAxisLineAlignment(AxisAlignment::Center)
);
bottommenu->setContentSize({ 35.f, 198.f });
 bottommenu->setPosition((17+4),186.000);
bottommenu->updateLayout();
		auto profile = this->getChildByID("profile-menu");
		 profile->setPosition((44+4),300.000);
		 	  profile->setScale(0.575);
		
		  auto profileUser = this->getChildByID("player-username");
		 profileUser->setPosition(-121212,121212);
		  profileUser->setScale(0.750);
		//  auto Newgrounds = this->getChildByID("newgrounds-button");
		// auto Sprite = Newgrounds->getChildren()->objectAtIndex(1);
		// Newgrounds->setScale(0.9);
		 auto ByeBye = this->getChildByID("social-media-menu");
		 ByeBye->setPosition(-121212,121212);
 				// profile->setAttribute("geode.mouse-api/tooltip", profileUser->getString() );
	if (Mainbooter) {
		FirstBoot = Mod::get()->getSettingValue<bool>("BootMessage");
		Mainbooter=false;
		};
if (FirstBoot) {
	FirstBoot=false;
	  auto alert = FLAlertLayer::create(
    "Warning! (From Better Menu)",    
    "<cr>This mod is Still in beta, Please report bugs if you find some.</c>", 
    "Sure!"    
); 
 alert->m_scene = this;
alert->show();
};
        return true; 
    }
};