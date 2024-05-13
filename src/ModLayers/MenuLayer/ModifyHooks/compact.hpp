#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include "../../Settings/Settings.hpp"
static CCMenuItemSpriteExtra* m_objectbettermenuhide = nullptr; // object->getParent() ??????
using namespace geode::prelude;
static void compactmenu(auto layer) {
if (!Mod::get()->getSettingValue<bool>("compact-main-menu")) {
    return;
};
auto winSize = CCDirector::get()->getWinSize();
auto compact = Build<CCMenu>::create()
                .pos(winSize.width - 90 ,  0) // 109
                .anchorPoint({0, 0})
                .scale(0.8)
                .parent(layer)
                .contentSize({109.000f, 48.108f})
                .layout(
                    RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::Center)
                    ->setGrowCrossAxis(false)
                    ->setCrossAxisReverse(true)
                    )
                .id("compact-bottom-menu"_spr)
                .collect();

auto geode = layer->getChildByIDRecursive("geode.loader/geode-button");
auto settings = layer->getChildByIDRecursive("settings-button");
if (geode) {
geode->removeFromParent();
compact->addChild(geode);
} // stops crashes
if (settings) {
settings->removeFromParent();
compact->addChild(settings);
} // stops crashes
auto btn = CircleButtonSprite::createWithSprite(
                "hide.png"_spr,
                1.15f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );

class pl : public CCLayer {
public:
void bettermenuhide(auto object) {
    bool enabled = m_objectbettermenuhide->getChildByIDRecursive("hiding"_spr)->isVisible();
        if (auto sc = m_objectbettermenuhide->getParent()) {
            if (auto layer = sc->getParent()) {
                layer->getChildByIDRecursive("bottom-menu")->setVisible(!enabled);
                if (layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) {layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)->setVisible(!enabled);};
                if (layer->getChildByIDRecursive("search-menu"_spr)) {layer->getChildByIDRecursive("search-menu"_spr)->setVisible(!enabled);};
                 if (layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)) {layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr)->setVisible(!enabled);};
                m_objectbettermenuhide->getChildByIDRecursive("hiding"_spr)->setVisible(!enabled);
                m_objectbettermenuhide->getChildByIDRecursive("showing"_spr)->setVisible(enabled);
            }
        }
	}
};

auto btnee = CCMenuItemSpriteExtra::create(btn, compact, menu_selector(pl::bettermenuhide));
m_objectbettermenuhide = btnee;
compact->addChild(btnee);

for(auto items : CCArrayExt<CCNode*>(btnee->getChildren())) {
			auto g=items;
			if (auto icon = getChildOfType<CCSprite>(g, 0)) {
        	auto store = CCSprite::create("show.png"_spr);
			store->setScale(icon->getScale());
			store->setPositionX(icon->getPositionX());
			store->setPositionY(icon->getPositionY());
			g->addChild(store);
            icon->setVisible(false);
            store->setID("showing"_spr);
            icon->setID("hiding"_spr);
			break;
    	    };
		 } // another way would prob easier
compact->updateLayout();


Build(layer->getChildByIDRecursive("right-side-menu"))
    .pos(winSize.width / 2,  28.190f)
    .scale(1);

auto searchmenu = layer->getChildByIDRecursive("shortcuts-menu-search"_spr);
auto searchmenufix = layer->getChildByIDRecursive("shortcuts-menu-Fix"_spr);
searchmenu->setPositionY(winSize.height - 80);
searchmenufix->setPositionY(winSize.height - 122);
layer->getChildByIDRecursive("bottom-menu")->setVisible(false);
int menupos = Mod::get()->getSettingValue<SettingPosStruct>("MenuPos-pos").m_pos;
if (menupos == 3) {
    layer->getChildByID("bottom-menu")->setPositionY(layer->getChildByID("bottom-menu")->getPositionY() - 50);
}
Build(layer->getChildByID("right-side-menu"))
        .layout(
                RowLayout::create()
                ->setGap(3.f)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setAutoScale(false)
                ->setGrowCrossAxis(false)
                ->setCrossAxisOverflow(true)
        ).updateLayout(); // quick way to make sure it is a RowLayout

Build(layer->getChildByID("profile-menu"))
                    .layout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start))
                    .pos(222222, winSize.height - 222222)
                    .scale(0.575f);

                Build(layer->getChildByID("player-username"))
                    .pos(222222,  winSize.height - 222222)
                    .anchorPoint({0.f,0.5});
return;
}