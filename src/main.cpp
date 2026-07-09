#include "MainScene.hpp"
#include <iostream>
#include "App.h"

int main() {
    MainScene scene{};

    App app{"Text Rendering"};
    app.loadScene(&scene);
    app.run();

    return 0;
}
