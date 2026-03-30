// from texture loader : https://github.com/geode-sdk/textureldr/blob/02e855839acc09f89b0b29f02d6123e2a91cf261/src/DragThingy.cpp
// with changes like DragThingy being sent and functions to add the callbacks after
#include "DragThingy.hpp"

#include <utility>

bool DragThingy::init(Function<void(DragThingy*)> onClick, Function<void(CCPoint, DragThingy*)> onMove, Function<void(DragThingy*)> onRelease) {
    if (!CCLayer::init()) return false;

    m_onClick = std::move(onClick);
    m_onMove = std::move(onMove);
    m_onRelease = std::move(onRelease);

    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);

    this->ignoreAnchorPointForPosition(false);

    return true;
}

bool DragThingy::ccTouchBegan(CCTouch* touch, CCEvent*) {
    auto const point = this->getParent()->convertToNodeSpace(touch->getLocation());
    auto const rect = this->boundingBox();
    if (rect.containsPoint(point)) {
        if (m_onClick)
            m_onClick(this);
        return true;
    }
    return false;
}

void DragThingy::ccTouchMoved(CCTouch* touch, CCEvent*) {
    if (m_onMove)
        m_onMove(touch->getDelta(), this);
}

void DragThingy::ccTouchEnded(CCTouch* touch, CCEvent*) {
    if (m_onRelease)
        m_onRelease(this);
}
void DragThingy::setDragCallback(Function<void(CCPoint, DragThingy*)> onMove) {
    m_onMove = std::move(onMove);
};
void DragThingy::setReleaseCallback(Function<void(DragThingy*)> onRelease) {
    m_onRelease = std::move(onRelease);
};
DragThingy* DragThingy::create(Function<void(DragThingy*)> onClick, Function<void(CCPoint, DragThingy*)> onMove, Function<void(DragThingy*)> onRelease) {
    auto ret = new DragThingy();
    if (ret->init(std::move(onClick), std::move(onMove), std::move(onRelease))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
