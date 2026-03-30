
#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenu.hpp>
#include <Geode/modify/CCMenuItem.hpp>
#include <Geode/modify/CCTouchDelegate.hpp>
#include <type_traits>
#include <variant>

class RFMMenu : public CCMenu {
  public:
	std::variant<std::monostate, CCTouchDelegate *, CCMenuItem *> m_pSelectedItem;
	static RFMMenu *createWithArray(CCArray *pArrayOfItems) {
		RFMMenu *pRet = new RFMMenu();
		if (pRet && pRet->initWithArray(pArrayOfItems)) {
			pRet->autorelease();
		} else {
			CC_SAFE_DELETE(pRet);
		}

		return pRet;
	};
	static RFMMenu *create() {
		return RFMMenu::create(NULL, NULL);
	}
	static RFMMenu *create(CCMenuItem *item, ...) {
		va_list args;
		va_start(args, item);

		RFMMenu *pRet = RFMMenu::createWithItems(item, args);

		va_end(args);

		return pRet;
	}
	static RFMMenu *createWithItems(CCMenuItem *item, va_list args) {
		CCArray *pArray = NULL;
		if (item) {
			pArray = CCArray::create(item, NULL);
			CCMenuItem *i = va_arg(args, CCMenuItem *);
			while (i) {
				pArray->addObject(i);
				i = va_arg(args, CCMenuItem *);
			}
		}

		return RFMMenu::createWithArray(pArray);
	}
	static RFMMenu *createWithItem(CCMenuItem *item) {
		return RFMMenu::create(item, NULL);
	}

	bool initWithArray(CCArray *pArrayOfItems) {
		if (CCMenu::initWithArray(pArrayOfItems)) {
			setTouchPriority(kCCMenuHandlerPriority - 1);
			return true;
		}
		return false;
	};
	void addChild(CCNode *child) override {
		if (auto touch = typeinfo_cast<CCTouchDelegate *>(child)) {
			CCLayer::addChild(child);
			CCDirector *pDirector = CCDirector::sharedDirector();
			pDirector->getTouchDispatcher()->removeDelegate(touch);
			return;
		} else if (auto menuItem = typeinfo_cast<CCMenuItem *>(child)) {
			/// hai
		} else {
			CCAssert(false, "Menu only supports CCTouchDelegate/CCMenuItem objects as children");
		}
		CCLayer::addChild(child);
	}
	void addChild(CCNode *child, int zOrder) override {
		if (auto touch = typeinfo_cast<CCTouchDelegate *>(child)) {
			CCLayer::addChild(child, zOrder);
			CCDirector *pDirector = CCDirector::sharedDirector();
			pDirector->getTouchDispatcher()->removeDelegate(touch);
			return;
		} else if (auto menuItem = typeinfo_cast<CCMenuItem *>(child)) {
			/// hai
		} else {
			CCAssert(false, "Menu only supports CCTouchDelegate/CCMenuItem objects as children");
		}
		CCLayer::addChild(child, zOrder);
	}
	void addChild(CCNode *child, int zOrder, int tag) override {
		if (auto touch = typeinfo_cast<CCTouchDelegate *>(child)) {
			CCLayer::addChild(child, zOrder, tag);
			CCDirector *pDirector = CCDirector::sharedDirector();
			pDirector->getTouchDispatcher()->removeDelegate(touch);
			return;
		} else if (auto menuItem = typeinfo_cast<CCMenuItem *>(child)) {
			/// hai
		} else {
			CCAssert(false, "Menu only supports CCTouchDelegate/CCMenuItem objects as children");
		}
		CCLayer::addChild(child, zOrder, tag);
	};
	void removeChild(CCNode *child, bool cleanup) override {

		if (auto touch = typeinfo_cast<CCTouchDelegate *>(child)) {
			if (auto selected = std::get_if<CCTouchDelegate *>(&m_pSelectedItem)) {
				if (*selected == touch) {
					m_pSelectedItem = std::monostate{};
				}
			}
		} else if (auto menuItem = typeinfo_cast<CCMenuItem *>(child)) {
			if (auto selected = std::get_if<CCMenuItem *>(&m_pSelectedItem)) {
				if (*selected == menuItem) {
					m_pSelectedItem = std::monostate{};
				}
			}
		} else {
			CCAssert(false, "Menu only supports CCTouchDelegate/CCMenuItem objects as children");
		}

		CCNode::removeChild(child, cleanup);
	};

	void handleTouchBegan(CCTouch *touch, CCEvent *event) {
		if (auto ptr = std::get_if<CCMenuItem *>(&m_pSelectedItem)) {
			if (*ptr)
				(*ptr)->selected();
		} else if (auto ptr = std::get_if<CCTouchDelegate *>(&m_pSelectedItem)) {
			if (*ptr)
				(*ptr)->ccTouchBegan(touch, event);
		}
	}

	void handleTouchMoved(CCTouch *touch, CCEvent *event) {
		if (auto ptr = std::get_if<CCMenuItem *>(&m_pSelectedItem)) {
			// none
		} else if (auto ptr = std::get_if<CCTouchDelegate *>(&m_pSelectedItem)) {
			if (*ptr)
				(*ptr)->ccTouchMoved(touch, event);
		}
	}

	void handleTouchEnded(CCTouch *touch, CCEvent *event) {
		if (auto ptr = std::get_if<CCMenuItem *>(&m_pSelectedItem)) {
			if (*ptr) {
				(*ptr)->unselected();
				(*ptr)->activate();
			}
		} else if (auto ptr = std::get_if<CCTouchDelegate *>(&m_pSelectedItem)) {
			if (*ptr)
				(*ptr)->ccTouchEnded(touch, event);
		}
	}

	void handleTouchCancelled(CCTouch *touch, CCEvent *event) {
		if (auto ptr = std::get_if<CCMenuItem *>(&m_pSelectedItem)) {
			if (*ptr)
				(*ptr)->unselected();
		} else if (auto ptr = std::get_if<CCTouchDelegate *>(&m_pSelectedItem)) {
			if (*ptr)
				(*ptr)->ccTouchCancelled(touch, event);
		}
	}
	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event) override {
		if (m_eState != kCCMenuStateWaiting || !m_bVisible || !m_bEnabled) {
			return false;
		}

		m_pSelectedItem = this->itemForTouch(touch);

		if (!std::holds_alternative<std::monostate>(m_pSelectedItem)) {
			m_eState = kCCMenuStateTrackingTouch;
			handleTouchBegan(touch, event);
			return true;
		}

		return false;
	}

	virtual void ccTouchEnded(CCTouch *touch, CCEvent *event) override {
		CCAssert(m_eState == kCCMenuStateTrackingTouch, "invalid state");

		handleTouchEnded(touch, event);

		m_pSelectedItem = std::monostate{};
		m_eState = kCCMenuStateWaiting;
	}

	virtual void ccTouchCancelled(CCTouch *touch, CCEvent *event) override {
		handleTouchCancelled(touch, event);
		m_pSelectedItem = std::monostate{};
		m_eState = kCCMenuStateWaiting;
	}

	virtual void ccTouchMoved(CCTouch *touch, CCEvent *event) override {
		CCAssert(m_eState == kCCMenuStateTrackingTouch, "invalid state");

		auto newItem = itemForTouch(touch);

		if (newItem != m_pSelectedItem) {
			handleTouchCancelled(touch, event);
			m_pSelectedItem = newItem;
			handleTouchBegan(touch, event);
		}

		handleTouchMoved(touch, event);
	}

	std::variant<std::monostate, CCTouchDelegate *, CCMenuItem *> itemForTouch(CCTouch *touch) {
		CCPoint touchLocation = touch->getLocation();

		for (auto node : CCArrayExt<CCNode>(m_pChildren)) {
			if (!node || !node->isVisible())
				continue;

			if (auto ccmenu = typeinfo_cast<CCMenu *>(node)) {
				if (!ccmenu->isEnabled()) continue;
			}

			CCPoint local = node->convertToNodeSpace(touchLocation);
			CCRect r = node->boundingBox();
			r.origin = CCPointZero;

			if (r.containsPoint(local)) {
				if (auto menuItem = typeinfo_cast<CCMenuItem *>(node)) {
					return menuItem;
				}
				if (auto touch = typeinfo_cast<CCTouchDelegate *>(node)) {
					return touch;
				}
			}
		}

		return std::monostate{};
	}
};