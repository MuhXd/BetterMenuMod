#ifdef GEODE_IS_WINDOWS
#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/cocos/menu_nodes/CCMenu.h>
#include <Geode/modify/CCMenu.hpp>
#include <Geode/utils/web.hpp>
using namespace geode::prelude;
class YoutubeButton : public CCLayer {
public:
void Link(CCObject*) {
		geode::utils::web::openLinkInBrowser("https://www.youtube.com/channel/UC3A8dtlO94rHEkn4Kz7aB7g");
	}
};

CCNode* menu = nullptr;
void inityoutubeandsuch() {
    geode::log::debug("Found ui");
};
inline bool isSpriteFrameName(CCNode* node, const char* name) {
    auto cache = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
    if (!cache) return false;

    auto* texture = cache->getTexture();
    auto rect = cache->getRect();

    if (auto* spr = typeinfo_cast<CCSprite*>(node)) {
        if (spr->getTexture() == texture && spr->getTextureRect() == rect) {
            return true;
        }
    } else if (auto* btn = typeinfo_cast<CCMenuItemSprite*>(node)) {
        auto* img = btn->getNormalImage();
        if (auto* spr = typeinfo_cast<CCSprite*>(img)) {
            if (spr->getTexture() == texture && spr->getTextureRect() == rect) {
                return true;
            }
        }
    }
    return false;
}

inline CCNode* getChildBySpriteFrameName(CCNode* parent, const char* name) {
    for (auto child : CCArrayExt<CCNode*>(parent->getChildren())) {
        if (::isSpriteFrameName(static_cast<CCNode*>(child), name)) {
            return child;
        }
    }
    return nullptr;
}

class $modify(CCMenu) {
    static void onModify(auto & self) { self.setHookPriority("CCMenu::addChild", 1000); };
    virtual void addChild(CCNode* Child, int zORDER, int tag) {
        CCMenu::addChild(Child, zORDER, tag);
        if (auto object = typeinfo_cast<CCMenuItemSpriteExtra*>(Child->getParent()->getChildren()->objectAtIndex(0))) {
            if (object == Child) {
                if (!object->getParent()) {
                    return;
                };
                
                if (menu == object->getParent()) {
                    return;
                }
                if (!object->getParent()->getParent()) {
                      return; 
                }
                auto cccccc = object->getParent()->getParent();
                if (!cccccc) {
                    return;
                }
            geode::Loader::get()->queueInMainThread([this,cccccc,object] {
            if (!this) {
                return;
            }
            if (!cccccc) {
                return;
            }
             if (!object) {
                return;
            }
            CCNode* a111111 = nullptr;
            for(auto items : CCArrayExt<CCNode*>(cccccc->getChildren())) {
				if (items->getChildrenCount() == 2) {
				a111111 = items;
				break;
    	    };
		}
                if (!a111111) {
                    return;
                }
                  if (a111111->getChildrenCount() < 2) {
                    return;
                  }
                cocos2d::CCNode* a22222 = typeinfo_cast<CCNode*>(a111111->getChildren()->objectAtIndex(0));
                 if (!a22222) {
                    return;
                }
                cocos2d::CCLabelBMFont* a333333 = getChildOfType<CCLabelBMFont>(a22222,0);
                 if (!a333333) {
                    return;
                }
                std::string id = a333333->getString();
                if (id == "(ID: muhammadgames.bettermenu)") {
            
                menu = typeinfo_cast<CCMenu*>(typeinfo_cast<CCNode*>(a22222->getChildren()->objectAtIndex(7))->getChildren()->objectAtIndex(1));

                inityoutubeandsuch();
                auto btn =  CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
                btn->setScale(0.7);
                auto btnee = CCMenuItemSpriteExtra::create(btn, menu, menu_selector(YoutubeButton::Link));
                btnee->setPosition(typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(0))->getPosition());
                typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(0))->removeFromParent();
                menu->addChild(btnee);
                 typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(2))->removeFromParent();
                } else { return; }
               });

            }
        }
    }
}; //geode.loader/homepage.png
#endif
