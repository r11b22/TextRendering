

#include "TextRendering/Text/TextMesh.hpp"
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

TextMesh::TextMesh(std::string name, AssetReference<Font> font, std::string text)
    : TransformableObject(name), MeshObject(name, {}, Material{"TextMaterial", glm::vec3{1.0f}}),
    mText(font),
    mFont(font)
{
    setShader("textShader");
    setRenderPass(0);

    mText.setText(std::move(text));
}


void TextMesh::onLoad() {
    Material atlasMaterial = {"Textmaterial", getAssetManager().getAsset(mFont)->getAtlas()};
    setMaterial(atlasMaterial);

    MeshReference meshRef = mText.createMesh(getAssetManager());
    setMesh(meshRef);
}

std::vector<RenderPassCommands> TextMesh::getRenderCommands() {
    StateChangeCommand state{};

    state.state.blend = true;
    state.state.srcBlendFactor = GL_SRC_ALPHA;
    state.state.dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;

    DrawCommand drawCommand{};

    drawCommand.mesh = getMesh();
    drawCommand.material = getMaterial();
    drawCommand.shaderName = getShader();

    Font* font = getAssetManager().getAsset(mFont);

    mText.setupUniforms(drawCommand, getAssetManager());


    InstanceData instance;
    instance.addUniform({"uModelMatrix", getTransformationMatrix()});

    drawCommand.instances.push_back(std::move(instance));


    return {RenderPassCommands{getRenderPass(), {state, drawCommand}}};
}
