#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <UIBuilder.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ModifyFiles.hpp"
using namespace geode::prelude;
class $modify(LoadingLayer) { 
     static void onModify(auto& self) {
        (void)self.setHookPriority("LoadingLayer::init", -1); // load somewhere
    }
     bool init(bool p0) {
        if (!LoadingLayer::init(p0))
            return false;

        SwelvyBGInsert(this,"LoadingLayer");
        return true;
     }
};