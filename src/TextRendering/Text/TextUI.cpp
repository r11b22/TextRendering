

#include "TextRendering/Text/TextUI.hpp"
#include "Buffer/Buffer.h"
#include "Defaults/Objects/Drawables/MeshObject.h"
#include "Defaults/Objects/TransformableObject.h"
#include "Material/Material.h"
#include "Mesh/MeshReference.hpp"
#include "Renderer/IDrawable.h"
#include "Renderer/Instancing/InstanceData.hpp"
#include "Renderer/RenderCommand.h"
#include "TextRendering/FontMetadata.hpp"
#include "glm/ext/vector_float3.hpp"
#include <vector>

TextUI::TextUI(std::string name, AssetReference<Font> font, std::string text)
    : TransformableObject(name), MeshObject(name, {}, Material{"TextMaterial", glm::vec3{1.0f}}),
    mText(font),
    mFont(font)
{
    setShader("textShader");
    setRenderPass(1);


    mText.setText(std::move(text));
}

void TextUI::setText(std::string text){
    mText.setText(std::move(text));
    MeshReference meshRef = mText.createMesh(getAssetManager().value());
    setMesh(meshRef);
}

void TextUI::setColor(glm::vec3 color){
    mText.setColor(std::move(color));
}

const glm::vec3& TextUI::getColor() const {
    return mText.getColor();
}


void TextUI::onLoad() {
    Material atlasMaterial = {"Textmaterial", getAssetManager().value().getAsset(mFont)->getAtlas()};
    setMaterial(atlasMaterial);

    MeshReference meshRef = mText.createMesh(getAssetManager().value());
    setMesh(meshRef);
}

std::vector<RenderPassCommands> TextUI::getRenderCommands() {
    StateChangeCommand state{};


    state.state.blend = true;
    state.state.srcBlendFactor = GL_SRC_ALPHA;
    state.state.dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;



    DrawCommand drawCommand{};

    drawCommand.mesh = getMesh();
    drawCommand.material = getMaterial();
    drawCommand.shaderName = getShader();

    Font* font = getAssetManager().value().getAsset(mFont);

    mText.setupUniforms(drawCommand, getAssetManager().value());

    InstanceData instance;
    instance.addUniform({"uModelMatrix", getTransformationMatrix()});

    drawCommand.instances.push_back(std::move(instance));


    return {RenderPassCommands{getRenderPass(), {state, drawCommand}}};
}
