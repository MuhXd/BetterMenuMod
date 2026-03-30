// from texture loader : https://github.com/geode-sdk/textureldr/blob/02e855839acc09f89b0b29f02d6123e2a91cf261/src/DragThingy.hpp#L1
#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class DragThingy : public CCLayer {
protected:
    bool init(Function<void(DragThingy*)> onClick, Function<void(CCPoint, DragThingy*)> onMove, Function<void(DragThingy*)> onRelease);

    bool ccTouchBegan(CCTouch* touch, CCEvent*) override;
    void ccTouchMoved(CCTouch* touch, CCEvent*) override;
    void ccTouchEnded(CCTouch* touch, CCEvent*) override;

public:
    // i need these public 3:
    Function<void(DragThingy*)> m_onClick;
    Function<void(CCPoint, DragThingy*)> m_onMove;
    Function<void(DragThingy*)> m_onRelease;
    //

    static DragThingy* create(Function<void(DragThingy*)> onClick, Function<void(CCPoint, DragThingy*)> onMove, Function<void(DragThingy*)> onRelease);
    
    // extra stuff
    void setDragCallback(Function<void(CCPoint, DragThingy*)> onMove);
    void setReleaseCallback(Function<void(DragThingy*)> onRelease);

};