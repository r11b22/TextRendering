

#include "MainScene.hpp"
#include "Asset/AssetLoader.hpp"
#include "Asset/AssetReference.hpp"
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
#include "TextRendering/FontLoader.hpp"

MainScene::MainScene(){
    AssetLoadInfo<Font> fontLoadInfo{};
    fontLoadInfo.name = "testFont";
    fontLoadInfo.atlasPath = "fonts/font-atlas/FreeSans.png";
    fontLoadInfo.metadataPath = "fonts/font-atlas/FreeSans.json";

    addAsset(fontLoadInfo);
}

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

    AssetReference<Font> font = getAssetManager().getAssetByName<Font>("testFont");

    ObjectReference<TextObject> text = createObject<TextObject>("test text", font, "Hello world");
    text->setPosition(glm::vec3{-2.5f, 0.0f, 0.0f});
}


MainScene::~MainScene(){
    delete mInputManager;
}
