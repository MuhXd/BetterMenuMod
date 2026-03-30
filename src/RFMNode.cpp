#include "RFMNode.hpp"

void RFMNode::selected() {
    if (this->m_callback != nullptr) CCMenuItemSpriteExtra::selected();
}
void RFMNode::unselected() {
    if (this->m_callback != nullptr) CCMenuItemSpriteExtra::unselected();
}

void RFMNode::activate() {
    if (!m_states.empty()) {
        m_stateIndex++;
        if (m_stateIndex >= m_states.size())
            m_stateIndex = 0;
        
        if (m_IsSpreadSheet) {
                if (auto ptr = std::get_if<const char*>(&m_sprites[m_stateIndex])) {
                    RFMNode::SetSpriteWithSheet(*ptr)->setScale(m_scaleStart);
                } else {
                    RFMNode::SetSprite(m_sprites[m_stateIndex])->setScale(m_scaleStart);
                }
            } else {
                RFMNode::SetSprite(m_sprites[m_stateIndex])->setScale(m_scaleStart);
            }
        if (this->m_callback != nullptr){
            this->m_callback(this, m_states[m_stateIndex]);
        };
    } else {
        if (this->m_callback != nullptr){
            this->m_callback(this, nullptr);
        };
    };

    // just in fucking case you somehow select without a m_callback
    CCMenuItemSpriteExtra::activate();
};
CCNode* RFMNode::SetSpriteScale(float scale) {
    m_scaleStart = scale;
    CCNode* sprite = this->getNormalImage();
    if (sprite) {
        sprite->setScale(scale);
    }
    return sprite;
};

CCSprite* RFMNode::SetSpriteWithSheet(const char* Texture) {
    auto CCSpr = CCSprite::createWithSpriteFrameName(Texture);
    if (CCSpr) {
        CCSpr->setScale(m_scaleStart);
        CCMenuItemSpriteExtra::setSprite(CCSpr);
    };
    return CCSpr;
};

CCNode* RFMNode::SetSprite(std::variant<const char *, CCNode *> Texture) {
    CCNode* CCSpr = NULL;
    if (auto ptr = std::get_if<const char*>(&Texture)) {
        CCSpr = CCSprite::create((*ptr));
    } else if (auto ptr = std::get_if<CCNode*>(&Texture)) {
         CCSpr = (*ptr);
    };
    if (CCSpr) {
        CCSpr->setScale(m_scaleStart);
        this->setNormalImage(CCSpr);
        this->updateSprite();
    };
    return CCSpr;
};
CCNode* RFMNode::SetSprite(std::variant<const char *, geode::Ref<CCNode>> Texture) {
    CCNode* CCSpr = NULL;
    if (auto ptr = std::get_if<const char*>(&Texture)) {
        CCSpr = CCSprite::create((*ptr));
    } else if (auto ptr = std::get_if<geode::Ref<CCNode>>(&Texture)) {
        if ( auto c = (*ptr).data() ) {CCSpr = c; }
        else {return nullptr;};
    };
    if (CCSpr) {
        CCSpr->setScale(m_scaleStart);
        this->setNormalImage(CCSpr);
        this->updateSprite();
    };
    return CCSpr;
};
// creates
RFMNode* RFMNode::create(std::variant<const char *, CCNode *> Texture) {
    auto sprite = new RFMNode();
    CCNode* CCSpr = NULL;
    if (auto ptr = std::get_if<const char*>(&Texture)) {
        CCSpr = CCSprite::create((*ptr));
    } else if (auto ptr = std::get_if<CCNode*>(&Texture)) {
         CCSpr = (*ptr);
    };
    if (CCSpr && sprite && sprite->init(CCSpr,CCSpr,CCSpr,nullptr))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

RFMNode* RFMNode::create(std::variant<const char*, CCNode *> Texture, std::function<void(RFMNode*, std::any)> menu) {
    auto spr = RFMNode::create(Texture);
    if (spr) {
        spr->m_callback = menu;
    }
    return spr;
}

RFMNode* RFMNode::createSheet(std::variant<const char*, CCNode *> Texture) {
    auto sprite = new RFMNode();
    CCNode* CCSpr = NULL;
    if (auto ptr = std::get_if<const char*>(&Texture)) {
        CCSpr = CCSprite::createWithSpriteFrameName((*ptr));
    } else if (auto ptr = std::get_if<CCNode*>(&Texture)) {
         CCSpr = (*ptr);
    };
    if (CCSpr && sprite && sprite->init(CCSpr,CCSpr,CCSpr,nullptr))
    {
        sprite->m_IsSpreadSheet = true;
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

RFMNode* RFMNode::createSheet(std::variant<const char*, CCNode *> Texture, std::function<void(RFMNode*, std::any)> menu) {
    auto spr = RFMNode::createSheet(Texture);
    if (spr) {
        spr->m_callback = menu;
    }
    return spr;
}