#pragma once
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

void PlaySoundEffect(auto effect){
	if (Mod::get()->getSettingValue<bool>("SFX")) {
		FMODAudioEngine::sharedEngine()->playEffect(effect);
	};
}

class FakeLayer : public CCLayer {
public:
void onGeode(CCObject*) {
		PlaySoundEffect("UwU_1.mp3"_spr);
		geode::openModsList();
	}
};

static void JokesMain(auto layer) {
	if (Mod::get()->getSettingValue<bool>("Geode-Server")) {
		 auto geodeButton = dynamic_cast<CCMenuItemSpriteExtra*>(
		layer->getChildByIDRecursive("geode.loader/geode-button")
	    );
		for(auto items : CCArrayExt<CCNode*>(layer->getChildByIDRecursive("geode.loader/geode-button")->getChildren())) {
			auto g=items;
				if (auto icon = getChildOfType<CCSprite>(g, 0)) {
				auto Spr = CCSprite::create("geodeservericon.png"_spr);
				Spr->setScale(0.2f);
				Spr->setPositionX(icon->getPositionX());
				Spr->setPositionY(icon->getPositionY());
				g->addChild(Spr);
				icon->setVisible(false);
				break;
    	    };
		}
		goto SkipUwu
	};
 	if (Mod::get()->getSettingValue<bool>("Uwugeode")) {
        auto geodeButton = dynamic_cast<CCMenuItemSpriteExtra*>(
		layer->getChildByIDRecursive("geode.loader/geode-button")
	    );

		geodeButton->m_pfnSelector = menu_selector(FakeLayer::onGeode);
		for(auto items : CCArrayExt<CCNode*>(layer->getChildByIDRecursive("geode.loader/geode-button")->getChildren())) {
			auto g=items;
				if (auto icon = getChildOfType<CCSprite>(g, 0)) {
				auto Spr = CCSprite::create("gwode.png"_spr);
				Spr->setScale(0.2f);
				Spr->setPositionX(icon->getPositionX());
				Spr->setPositionY(icon->getPositionY());
				g->addChild(Spr);
				icon->setVisible(false);
				break;
    	    };
		}
	};
	SkipUwu:
	if (Mod::get()->getSettingValue<bool>("Merp")) {
		auto title = layer->getChildByID("main-title");
		auto titlePos = title->getPosition();
		auto Spr = CCSprite::create("MERP.png"_spr);
		Spr->setPositionX(titlePos.x);
		Spr->setPositionY(titlePos.y);
		layer->removeChildByID("main-title");
		Spr->setID("main-title");
		layer->addChild(Spr);
	}
}