#include <Geode/Geode.hpp>
#include <vector>

using namespace geode::prelude;

class RFMVisibilityActions : public cocos2d::CCNode {
private:
    struct Entry {
        geode::WeakRef<CCNode> node;
        bool Fired;
        std::function<void(CCNode*)> onVisible;
        std::function<void(CCNode*)> onInvisible;
    };

    std::vector<Entry> m_nodes;

public:
    static RFMVisibilityActions* create() {
        auto ret = new RFMVisibilityActions();
        if (ret && ret->init()) {
            ret->scheduleUpdate();
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void add(CCNode* node,std::function<void(CCNode*)> enter,std::function<void(CCNode*)> exit) {

        m_nodes.push_back({
            node,
            node->isVisible(),
            std::move(enter),
            std::move(exit)
        });

        if (node->isVisible()) {
            if (enter) exit(node);
        } else {
            if (exit) enter(node);
        }
    }

    void update(float) override {
        for (auto it = m_nodes.begin(); it != m_nodes.end();) {

            auto node = it->node.lock();

            if (!node) {
                it = m_nodes.erase(it);
                continue;
            }

            if (it->Fired != node->isVisible()) {
                it->Fired = node->isVisible();

                if (it->Fired) {
                    if (it->onVisible) it->onVisible(node);
                } else {
                    if (it->onInvisible) it->onInvisible(node);
                }
            }

            ++it;
        }
    }
};