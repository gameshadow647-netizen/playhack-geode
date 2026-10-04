#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Глобальная переменная для отслеживания состояния FPS
bool g_showFpsCounter = false;

// Класс для нашего всплывающего чит-оконца
class PlayHackMenu : public FLAlertLayer {
public:
    CCLabelBMFont* m_statusLabel = nullptr;

    static PlayHackMenu* create() {
        auto ret = new PlayHackMenu();
        // В FLAlertLayer инициализация init() не принимает аргументов напрямую, настраиваем через кастомный метод
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() {
        if (!FLAlertLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Главный контейнер для элементов окна
        m_mainLayer = CCLayer::create();
        this->addChild(m_mainLayer);

        // Задний фон меню (размер 300x200 пикселей)
        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({ 300, 200 });
        bg->setPosition(winSize / 2);
        m_mainLayer->addChild(bg);

        // Заголовок нашего чит-меню
        auto title = CCLabelBMFont::create("PlayHack Menu", "goldFont.fnt");
        title->setPosition({ winSize.width / 2, winSize.height / 2 + 75 });
        title->setScale(0.8f);
        m_mainLayer->addChild(title);

        // Создаем контейнер для кнопок
        auto menu = CCMenu::create();
        menu->setPosition({ 0, 0 });
        m_mainLayer->addChild(menu);

        // КНОПКА 1: Переключатель FPS
        auto btnText1 = CCLabelBMFont::create("Toggle FPS", "bigFont.fnt");
        btnText1->setScale(0.4f);
        auto btn1 = CCMenuItemSpriteExtra::create(
            btnText1, this, menu_selector(PlayHackMenu::onToggleFps)
        );
        btn1->setPosition({ winSize.width / 2, winSize.height / 2 + 20 });
        menu->addChild(btn1);

        // Текст статуса под кнопкой FPS
        m_statusLabel = CCLabelBMFont::create(g_showFpsCounter ? "FPS: ON" : "FPS: OFF", "chatFont.fnt");
        m_statusLabel->setPosition({ winSize.width / 2, winSize.height / 2 });
        m_statusLabel->setScale(0.5f);
        m_mainLayer->addChild(m_statusLabel);

        // КНОПКА 2: Закрыть меню (Крестик)
        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(
            closeSprite, this, menu_selector(PlayHackMenu::onClose)
        );
        closeBtn->setPosition({ winSize.width / 2 + 135, winSize.height / 2 + 85 });
        menu->addChild(closeBtn);

        // Разрешаем клики
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
    }

    void onToggleFps(CCObject*) {
        // Переключаем значение (вкл/выкл)
        g_showFpsCounter = !g_showFpsCounter;
        
        // Обновляем текст в меню
        if (m_statusLabel) {
            m_statusLabel->setString(g_showFpsCounter ? "FPS: ON" : "FPS: OFF");
        }
    }

    void onClose(CCObject*) {
        this->removeFromParentAndCleanup(true);
    }
};

// Внедряем кнопку вызова меню и логику счетчика в саму игру
class $modify(PlayHackLayer, PlayLayer) {
    CCLabelBMFont* m_fpsCounterLabel = nullptr;

    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // 1. Создаем кнопку "PlayHack" на экране уровня
        auto menu = CCMenu::create();
        menu->setPosition({ 0, 0 });
        this->addChild(menu);

        auto btnText = CCLabelBMFont::create("PlayHack", "chatFont.fnt");
        btnText->setScale(0.6f);
        
        auto hackMenuBtn = CCMenuItemSpriteExtra::create(
            btnText, this, menu_selector(PlayHackLayer::openHackMenu)
        );
        
        hackMenuBtn->setPosition({ 40, winSize.height - 20 });
        menu->addChild(hackMenuBtn);

        // 2. Создаем сам счетчик FPS, который будет обновляться
        m_fields->m_fpsCounterLabel = CCLabelBMFont::create("FPS: ---", "chatFont.fnt");
        m_fields->m_fpsCounterLabel->setPosition({ 10, 10 });
        m_fields->m_fpsCounterLabel->setAnchorPoint({ 0, 0 });
        m_fields->m_fpsCounterLabel->setScale(0.5f);
        m_fields->m_fpsCounterLabel->setOpacity(150);
        m_fields->m_fpsCounterLabel->setVisible(g_showFpsCounter);
        this->addChild(m_fields->m_fpsCounterLabel);

        // Включаем обновление каждый кадр
        this->scheduleUpdate();

        return true;
    }

    // Этот метод вызывается каждый кадр игры
    void update(float dt) {
        PlayLayer::update(dt);

        if (m_fields->m_fpsCounterLabel) {
            // Показываем или скрываем счетчик в зависимости от кнопки в меню
            m_fields->m_fpsCounterLabel->setVisible(g_showFpsCounter);

            if (g_showFpsCounter && dt > 0) {
                // Считаем кадры в секунду
                int fps = static_cast<int>(1.0f / dt);
                m_fields->m_fpsCounterLabel->setString(ccsprintf("FPS: %d", fps));
            }
        }
    }

    void openHackMenu(CCObject*) {
        auto popup = PlayHackMenu::create();
        // Правильный метод добавления всплывающего слоя в Geode/Cocos2d
        CCDirector::sharedDirector()->getRunningScene()->addChild(popup, 100);
    }
};
