#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
#include <type_traits>
#include <variant>

using namespace geode::prelude;
inline std::variant<const char*, geode::Ref<CCNode>> convert(
    std::variant<const char*, CCNode*> const& input
) {
    if (auto str = std::get_if<const char*>(&input)) {
        return *str;
    }
    if (auto node = std::get_if<CCNode*>(&input)) {
        return geode::Ref<CCNode>(*node);
    }
    return nullptr;
};

#define RFM_STATE_SETUP() \
    static_assert(sizeof...(States) == sizeof...(textures), \
        "Number of states must match number of textures"); \
    std::variant<const char*, CCNode*> tex[] = {textures...}; \
    T values[] = {States...};

#define RFM_STATE_PUSH(node) \
    for (size_t i = 0; i < sizeof...(States); i++) { \
        node->m_states.push_back(values[i]); \
        node->m_sprites.push_back(convert(tex[i])); \
    }

#define RFM_CREATE_TEMPLATE(NAME, BASECALL) \
template<typename T, T... States, typename... Sprites> \
static RFMNode* NAME(Sprites... textures) { \
    RFM_STATE_SETUP() \
    auto node = BASECALL(tex[0]); \
    if (!node) return nullptr; \
    RFM_STATE_PUSH(node) \
    return node; \
};

#define RFM_CREATE_TEMPLATE_CB(NAME, BASECALL) \
template<typename T, T... States, typename Callback, typename... Sprites> \
static RFMNode* NAME(Callback callback, Sprites... textures) { \
    RFM_STATE_SETUP() \
    auto node = BASECALL(tex[0],[callback](RFMNode* n, std::any v){ \
            callback(n, std::any_cast<T>(v)); \
        } \
    ); \
    if (!node) return nullptr; \
    RFM_STATE_PUSH(node) \
    return node; \
};

class RFMNode : public CCMenuItemSpriteExtra {
public:
    static RFMNode* create(std::variant<const char *, CCNode *> Texture);
    static RFMNode* create(std::variant<const char *, CCNode *> Texture, std::function<void(RFMNode*, std::any)> menu);

    static RFMNode* createSheet(std::variant<const char*, CCNode *> Texture);
    static RFMNode* createSheet(std::variant<const char*, CCNode *> Texture, std::function<void(RFMNode*, std::any)> menu);

    RFM_CREATE_TEMPLATE(create, RFMNode::create)
    RFM_CREATE_TEMPLATE_CB(createWithCallback, RFMNode::create)

    RFM_CREATE_TEMPLATE(createSheet, RFMNode::createSheet)

    RFM_CREATE_TEMPLATE_CB(createSheetWithCallback, RFMNode::createSheet)

    // utils
    CCSprite* SetSpriteWithSheet(const char* Texture);
    CCNode* SetSprite(std::variant<const char *, CCNode *> Texture);
     CCNode* SetSprite(std::variant<const char *, geode::Ref<CCNode>> Texture);
    CCNode* SetSpriteScale(float scale);

    virtual void selected();
    virtual void activate();
    virtual void unselected();

    template<typename T>
    void setValue(T value) {
        for (size_t i = 0; i < m_states.size(); ++i) {
                if (std::any_cast<T>(m_states[i]) == value) {
                    m_stateIndex = i;
                    if (m_IsSpreadSheet) {
                        if (auto ptr = std::get_if<const char*>(&m_sprites[m_stateIndex])) {
                            RFMNode::SetSpriteWithSheet(*ptr)->setScale(m_scaleStart);
                        } else {
                             RFMNode::SetSprite(m_sprites[m_stateIndex])->setScale(m_scaleStart);
                        }
                    } else {
                        RFMNode::SetSprite(m_sprites[m_stateIndex])->setScale(m_scaleStart);
                    }
                    return; 
                }
        }
        log::warn("Value not found in m_states");
    }

    std::function<void(RFMNode*, std::any)> m_callback = nullptr;

    std::vector<std::any> m_states;
    std::vector<std::variant<const char*, geode::Ref<CCNode>>> m_sprites;
    float m_scaleStart = 1;
    bool m_IsSpreadSheet = false;

    size_t m_stateIndex = 0;
};