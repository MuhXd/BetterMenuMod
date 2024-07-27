
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/modify/CCSprite.hpp>
using namespace geode::prelude;
class YoutubeButton : public CCLayer {
public:
void LinkYoutube(CCObject*) {
		geode::utils::web::openLinkInBrowser("https://www.youtube.com/channel/UC3A8dtlO94rHEkn4Kz7aB7g");
	}
    void LinkTwitch(CCObject*) {
		geode::utils::web::openLinkInBrowser("https://www.twitch.tv/muhammad_mo");
	}
};

CCNode* menuNoDupe = nullptr;
 //geode.loader/gift.png

class $modify(CCSprite) {
    static void onModify(auto & self) { (void)self.setHookPriority("CCSprite::createWithSpriteFrameName", 1000); }

    static CCSprite* createWithSpriteFrameName(char const* frameName) {
        CCSprite* sprite = CCSprite::createWithSpriteFrameName(frameName);
        std::string sprname = frameName;
        if (sprname == "geode.loader/gift.png") {
            //geode::log::debug("0 : {}",sprname);
              geode::Loader::get()->queueInMainThread([frameName,sprite] {
                if (!sprite) {
                    return;
                }
                if (auto btn = sprite->getParent()) {
                    //geode::log::debug("1 : {}",frameName);
                    if (auto menu = btn->getParent()) {
                        // geode::log::debug("2 : {}",frameName);
                        if (auto Dump = menu->getParent()) {
                            //geode::log::debug("3 : {}",frameName);
                            if (menuNoDupe == Dump) return;
                            menuNoDupe = Dump;
                            if (auto Dump1 = Dump->getParent()) {
                                 //geode::log::debug("4 : {}",frameName);
                                 cocos2d::CCLabelBMFont* a333333 = getChildOfType<CCLabelBMFont>(Dump1,0);
                                 if (!a333333) {
                                      return;
                                }
                                std::string id = a333333->getString();
                                 if (id == "(ID: muhammadgames.bettermenu)") {
                                    auto btn =  CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
                                    btn->setScale(0.7);
                                    auto btnee = CCMenuItemSpriteExtra::create(btn, menu, menu_selector(YoutubeButton::LinkYoutube));
                                    btnee->setPosition(typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(0))->getPosition());
                                    typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(0))->removeFromParent();
                                    menu->addChild(btnee);
                                    typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(2))->removeFromParent();
                                    auto weed2 =  CCSprite::createWithSpriteFrameName("gj_twitchIcon_001.png");
                                    weed2->setScale(0.7);
                                    auto btneeweed2f = CCMenuItemSpriteExtra::create(weed2, menu, menu_selector(YoutubeButton::LinkTwitch));
                                    btneeweed2f->setPosition(typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(1))->getPosition());
                                    typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(1))->removeFromParent();
                                    menu->addChild(btneeweed2f);
                                    } 
                                }
                            }
                        }
                    }
                    });
                };

        
        return sprite;
    }
};