#pragma once

#include "InputManager.h"
#include "Scene/Scene.hpp"

class MainScene : public Scene{
    private:
        InputManager* mInputManager = nullptr;
    public:
        MainScene();
        ~MainScene();
        void onLoad(Renderer& renderer, Window &window) override;
    private:
};
