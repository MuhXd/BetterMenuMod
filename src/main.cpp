#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/binding/EndLevelLayer.hpp>
#include <Geode/cocos/base_nodes/Layout.hpp> 

// TOP RIGHT CLOSE
// to LEFT PROFILE FUNNI
using namespace geode::prelude;
auto Mainbooter = true;
auto FirstBoot = false;
auto macosdisableThingsthatnoworkyonemacy=false
struct $modify(newer,MenuLayer) {
	#ifdef GEODE_IS_MACOS {
	macosdisableThingsthatnoworkyonemacy = true
}
if (!macosdisableThingsthatnoworkyonemacy) {
  	void onMyLevelsClick(CCObject* target) {
auto CreatorLayer = CreatorLayer::create();
	CreatorLayer->onMyLevels(target);
  }
  	void SearchButtonClicked(CCObject* target) {
auto CreatorLayer = CreatorLayer::create();
	CreatorLayer->onOnlineLevels(target);
  }
    	void onSavedLevelsClick(CCObject* target) {
auto CreatorLayer = CreatorLayer::create();
	CreatorLayer->onSavedLevels(target);
  }
}
    bool init() {
        if (!MenuLayer::init() ) {
            return false;
		};
		if (!Mod::get()->getSettingValue<bool>("RunMainMenu")) {
			return true;
		};
if (Mod::get()->getSettingValue<bool>("MoveMenuPos")) {
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
		 profileUser->setPosition(76,391);
		   profileUser->setScale(1);
};

if (Mod::get()->getSettingValue<bool>("HideName")) {
		  auto profileUser = this->getChildByID("player-username");
		 profileUser->setPosition(-121212,121212);
		  profileUser->setScale(0);
};
		 auto ByeBye = this->getChildByID("social-media-menu");
		 ByeBye->setPosition(-121212,121212);
		   if (macosdisableThingsthatnoworkyonemacy) {
      auto menu = CCMenu::create();
	  int Offset = 0;
	  // Shortcut Search thingy
	   if (Mod::get()->getSettingValue<bool>("ShortcutSearch")) {
	  auto SearchIcon = CCSprite::createWithSpriteFrameName("GJ_searchBtn_001.png");
        SearchIcon->setScale(0.7f);
        auto Search = CCMenuItemSpriteExtra::create(SearchIcon, this, menu_selector(newer::SearchButtonClicked));
      	auto more=this->getChildByID("more-games-menu");
		menu->addChild(Search);
		more-> setPosition(-122222,-120);
        Search->setPosition(505.875,19.500);
        Search->setID("bettermenu/Search/MoreGamesReplacement");
	   }
	  // accountBtn_myLevels_001.png
	  // GJ_editBtn_001.png
	  // 1
	
	  if (Mod::get()->getSettingValue<bool>("ShortcutMyLevel")) {
		this->addChild(menu); 
        auto Profiler = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
        Profiler->setScale(0.4f);
        auto onMyLevels = CCMenuItemSpriteExtra::create(Profiler, this, menu_selector(newer::onMyLevelsClick));
      	menu->addChild(onMyLevels);
  
       menu->setPosition((17+4+Offset),22);
	   menu->setContentSize({ 0, 0 });
	   menu->setAnchorPoint({0,0});
        onMyLevels->setPosition(0,0);
        onMyLevels->setID("bettermenu/shortcut/onMyLevels");
Offset=Offset+5+onMyLevels->getContentSize().width;
	  }
// 2
 if (Mod::get()->getSettingValue<bool>("ShortcutSavedLevels")) {
	this->addChild(menu); 
        auto onSavedLevelsIcon = CCSprite::createWithSpriteFrameName("accountBtn_myLevels_001.png");
        onSavedLevelsIcon->setScale(0.7f);
        auto onSavedLevels = CCMenuItemSpriteExtra::create(onSavedLevelsIcon, this, menu_selector(newer::onSavedLevelsClick));
      	menu->addChild(onSavedLevels);
        onSavedLevels->setPosition(0+Offset,0);
        onSavedLevels->setID("bettermenu/shortcut/onSavedLevels");
		Offset=Offset+5+onSavedLevels->getContentSize().width;
 }
//
	  }

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

/*
	  auto alert = FLAlertLayer::create(
    "You what what that means",    
    "<cb>FISH</c>", 
    ""    
); 
 alert->m_scene = this;
alert->show();
*/
};
        return true; 
    }
};