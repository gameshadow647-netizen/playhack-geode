#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayHackLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;

        auto menu = CCMenu::create();
        menu->setPosition({ 0, 0 });
        this->addChild(menu);

        auto btnText = CCLabelBMFont::create("PlayHack", "chatFont.fnt");
        btnText->setScale(0.6f);
        
        auto hackMenuBtn = CCMenuItemSpriteExtra::create(
            btnText, this, menu_selector(PlayHackLayer::openHackMenu)
        );
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        hackMenuBtn->setPosition({ 40, winSize.height - 20 });
        menu->addChild(hackMenuBtn);

        return true;
    }

    void openHackMenu(CCObject*) {
        FLAlertLayer::create("PlayHack", "Menu is working!", "OK")->show();
    }
};
