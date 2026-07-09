

#include "MainScene.hpp"
#include "Defaults/Camera/FirstPersonCamera.h"
#include "Defaults/Objects/Lighting/AmbientLight.h"
#include "FileReader.h"
#include "Object/ObjectRepository.h"
#include "Renderer/Renderer.h"
#include "Shader/ShaderProgram.h"
#include "TextRendering/FontLoader.hpp"
#include "TextRendering/TextObject.hpp"
#include "Window.h"
#include "glm/ext/vector_float3.hpp"
#include <memory>

void MainScene::onLoad(Renderer& renderer, Window& window){
    std::unique_ptr<ShaderProgram> textShader = std::make_unique<ShaderProgram>();
    textShader->addShader(FileReader::readFile("Shaders/textShader/vertex.glsl").c_str(), GL_VERTEX_SHADER);
    textShader->addShader(FileReader::readFile("Shaders/textShader/fragment.glsl").c_str(), GL_FRAGMENT_SHADER);
    textShader->link();

    renderer.addShaderProgram("textShader", std::move(textShader));

    mInputManager = new InputManager(window);


    ObjectReference<FirstPersonCamera> camera = createObject<FirstPersonCamera>("Camera", mInputManager, &window);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));



    FontLoader loader{};

    Font font = loader.readFiles("fonts/font-atlas/FreeSans.png", "fonts/font-atlas/FreeSans.json", getAssetManager());

    ObjectReference<TextObject> mText = createObject<TextObject>("test text", font, "Hello World!");
}


MainScene::~MainScene(){
    delete mInputManager;
}
