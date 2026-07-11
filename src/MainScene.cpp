

#include "MainScene.hpp"
#include "Asset/AssetLoader.hpp"
#include "Asset/AssetReference.hpp"
#include "Defaults/Camera/FirstPersonCamera.h"
#include "Defaults/Objects/Lighting/AmbientLight.h"
#include "FileReader.h"
#include "Object/ObjectRepository.h"
#include "Renderer/RenderPass.hpp"
#include "Renderer/Renderer.h"
#include "Shader/ShaderProgram.h"
#include "TextRendering/FontLoader.hpp"
#include "TextRendering/Text/TextMesh.hpp"
#include "TextRendering/Text/TextUI.hpp"
#include "Window.h"
#include "glm/ext/vector_float3.hpp"
#include <format>
#include <memory>
#include "TextRendering/FontLoader.hpp"

MainScene::MainScene(){
    AssetLoadInfo<Font> fontLoadInfo{};
    fontLoadInfo.name = "testFont";
    fontLoadInfo.atlasPath = "fonts/font-atlas/FreeSans.png";
    fontLoadInfo.metadataPath = "fonts/font-atlas/FreeSans.json";

    addAsset(fontLoadInfo);

    createRenderPass();
    getRenderPass(1).setProjectionType(ProjectionType::Orthographic);
    getRenderPass(1).setCameraType(CameraType::UI);
}

void MainScene::onLoad(Renderer& renderer, Window& window){
    window.setVSYNC(false);

    std::unique_ptr<ShaderProgram> textShader = std::make_unique<ShaderProgram>();
    textShader->addShader(FileReader::readFile("Shaders/textShader/vertex.glsl").c_str(), GL_VERTEX_SHADER);
    textShader->addShader(FileReader::readFile("Shaders/textShader/fragment.glsl").c_str(), GL_FRAGMENT_SHADER);
    textShader->link();

    renderer.addShaderProgram("textShader", std::move(textShader));

    mInputManager = new InputManager(window);


    ObjectReference<FirstPersonCamera> camera = createObject<FirstPersonCamera>("Camera", mInputManager, &window);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));


    AssetReference<Font> font = getAssetManager().getAssetByName<Font>("testFont");

    ObjectReference<TextMesh> text = createObject<TextMesh>("test text", font, "World text.");
    text->setPosition(glm::vec3{-2.5f, 0.0f, 0.0f});


    mFPSCounter = createObject<TextUI>("fps counter", font, "fps counter");
    mFPSCounter->setPosition(glm::vec3{-400.0f, 200.0f, 0.0f});
    mFPSCounter->setScale(glm::vec3{20.0f});

    mUpdatingTest = createObject<TextUI>("update text", font, "");
    mUpdatingTest->setPosition(glm::vec3{200.0f, 200.0f, 0.0f});
    mUpdatingTest->setScale(glm::vec3{20.0f});
    mUpdatingTest->setColor(glm::vec3{0.0f, 1.0f, 1.0f});

}

void MainScene::onUpdate(Renderer& renderer, Window& window, float deltaT) {
    float FPS = 1/deltaT;
    float MS = deltaT*1000;
    if(mUpdateCounter >= mUpdateTime){
        mFPSCounter->setText(std::format("FPS: {:.0f}, Frame Time: {:.2f}ms", FPS, MS));
        mUpdateCounter = 0;
    }
    mUpdateCounter += deltaT;

    mUpdatingTest->setText(std::format("{}", mUpdateCounter));
}


MainScene::~MainScene(){
    delete mInputManager;
}
