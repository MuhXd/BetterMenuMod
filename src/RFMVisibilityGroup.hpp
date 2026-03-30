class RFMVisibilityGroup : public cocos2d::CCNode {
private:
    struct Entry {
        geode::WeakRef<CCNode> node;
        bool intendedVisible;
    };

    std::vector<Entry> m_nodes;

public:
    static RFMVisibilityGroup* create() {
        auto ret = new RFMVisibilityGroup();
        if (ret && ret->init()) {
            ret->scheduleUpdate();
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void add(CCNode* node) {
        m_nodes.push_back({ node, node->isVisible() });
    }

    void setVisible(bool visible) override {
        m_bVisible = visible;

        for (auto& entry : m_nodes) {
            if (auto node = entry.node.lock()) {
                node->setVisible(visible ? entry.intendedVisible : false);
            }
        }
    }

    void update(float) override {
        for (auto it = m_nodes.begin(); it != m_nodes.end();) {

            auto node = it->node.lock();

            if (!node) {
                it = m_nodes.erase(it);
                continue;
            }

            if (!m_bVisible) {
                node->setVisible(false);
            } else {
                if (node->isVisible() != it->intendedVisible) {
                    it->intendedVisible = node->isVisible();
                }

                node->setVisible(it->intendedVisible);
            }

            ++it;
        }
    }
};