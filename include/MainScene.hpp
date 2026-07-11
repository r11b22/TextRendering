#pragma once

#include "InputManager.h"
#include "Object/ObjectRepository.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.hpp"
#include "TextRendering/Text/TextUI.hpp"

class MainScene : public Scene{
    private:
        InputManager* mInputManager = nullptr;

        ObjectReference<TextUI> mFPSCounter;

        float mUpdateCounter = 0;
        float mUpdateTime = 2;
    public:
        MainScene();
        ~MainScene();
        void onLoad(Renderer& renderer, Window &window) override;

        void onUpdate(Renderer& renderer, Window& window, float deltaT) override;
    private:
};
