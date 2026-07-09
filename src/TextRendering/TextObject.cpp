

#include "TextRendering/TextObject.hpp"
#include "Buffer/Buffer.h"
#include "Defaults/Objects/Drawables/MeshObject.h"
#include "Defaults/Objects/TransformableObject.h"
#include "Material/Material.h"
#include "Mesh/MeshReference.hpp"
#include "Renderer/Instancing/InstanceData.hpp"
#include "Renderer/RenderCommand.h"
#include "TextRendering/FontMetadata.hpp"
#include "TextRendering/GlyphMetadata.hpp"
#include "glm/ext/vector_float3.hpp"
#include <vector>

TextObject::TextObject(std::string name, Font font, std::string text)
    : TransformableObject(name), MeshObject(name, {}, Material{"TextMaterial", glm::vec3{1.0f}}),
    mText(std::move(text)),
    mFont(font)
{
    setShader("textShader");
    Material atlasMaterial = {"Textmaterial", mFont.getAtlas()};
    setMaterial(atlasMaterial);
}


void TextObject::onLoad() {
    createMesh();
}

std::vector<RenderCommand> TextObject::getRenderCommands() {
    StateChangeCommand state{};

    state.state.blend = true;
    state.state.srcBlendFactor = GL_SRC_ALPHA;
    state.state.dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;

    DrawCommand drawCommand{};

    drawCommand.mesh = getMesh();
    drawCommand.material = getMaterial();
    drawCommand.shaderName = getShader();

    drawCommand.staticUniforms.push_back({"uDistanceRange", mFont.getMetadata().getDistanceRange()});
    drawCommand.staticUniforms.push_back({"uDistanceRangeMiddle", mFont.getMetadata().getDistanceRangeMiddle()});

    drawCommand.staticUniforms.push_back({"uTextColor", glm::vec3{1.0f}});


    InstanceData instance;
    instance.addUniform({"uModelMatrix", getTransformationMatrix()});

    drawCommand.instances.push_back(std::move(instance));


    return {state, drawCommand};
}


void TextObject::createMesh() {

    // Setup start vertex
    mVertices = {};

    // Setup start indices
    mIndices = {};

    mIndexOffset = 0;
    mVertexOffset = 0;
    mLastAdvance = 0;
    for (const auto& character : mText){
        createSingleCharacter(character);
    }


    Mesh mesh{"textMesh"};

    mesh.setVertices(mVertices);
    mesh.setIndices(mIndices);

    MeshReference meshRef = getAssetManager().addAsset(std::move(mesh));

    setMesh(meshRef);
}

void TextObject::createSingleCharacter(size_t character){
    mVertexOffset += mLastAdvance;


    const GlyphMetadata& glyph = mFont.getMetadata().getGlyph(character);



    const GlyphVertices vertices = glyph.getPlaneBoundVertices();
    const GlyphUVs uvs = mFont.getMetadata().getGlyphUVs(character);
    std::vector<float> newVertices = {
        // left
        mVertexOffset + vertices.bottomLeft.x, vertices.bottomLeft.y,       0.0f, 0.0f, 0.0f, 1.0f,   uvs.bottomLeft.x, uvs.bottomLeft.y, // 0: Bottom-Left
        mVertexOffset + vertices.topLeft.x,  vertices.topLeft.y,            0.0f, 0.0f, 0.0f, 1.0f,   uvs.topLeft.x, uvs.topLeft.y, // 1: Top-Left

        // right
        mVertexOffset + vertices.bottomRight.x, vertices.bottomRight.y,     0.0f, 0.0f, 0.0f, 1.0f,   uvs.bottomRight.x, uvs.bottomRight.y,
        mVertexOffset + vertices.topRight.x,  vertices.topRight.y,          0.0f, 0.0f, 0.0f, 1.0f,   uvs.topRight.x, uvs.topRight.y
    };

    mVertices.insert(mVertices.end(), newVertices.begin(), newVertices.end());

    std::vector<unsigned int> newIndices = {
        mIndexOffset + 0, mIndexOffset + 2, mIndexOffset + 3, // Triangle 1: Bottom-Left -> Bottom-Right -> Top-Right
        mIndexOffset + 3, mIndexOffset + 1, mIndexOffset + 0  // Triangle 2: Top-Right -> Top-Left -> Bottom-Left
    };
    mIndices.insert(mIndices.end(), newIndices.begin(), newIndices.end());


    mIndexOffset += 4;

    mLastAdvance = glyph.getAdvance();
}
