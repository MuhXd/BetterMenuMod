#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/EndLevelLayer.hpp>
#include <Geode/cocos/base_nodes/Layout.hpp>

using namespace geode::prelude;
auto FirstBoot = true;
struct $modify(MenuLayer) {
	

    bool init() {
        if (!MenuLayer::init())
            return false;


        auto winSize = CCDirector::get()->getWinSize();
        auto title = this->getChildByID("main-title");
		title->setPosition(73.500, 79);
		  title->setScale(0.35);
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
 bottommenu->setPosition(15.500,189.000);
bottommenu->updateLayout();
		auto profile = this->getChildByID("profile-menu");
		 profile->setPosition(223.000,289.000);
		  auto profileUser = this->getChildByID("player-username");
		 profileUser->setPosition(298,289);
		  profileUser->setScale(0.750);

if (FirstBoot) {
	FirstBoot=false;
	  auto alert = FLAlertLayer::create(
    "Warning!",    
    "<cr>This mod is Still in beta, Please report bugs if you find some.</c>", 
    "Sure!"    
);
 alert->m_scene = this;
alert->show();
};
        return true; 
    }
};