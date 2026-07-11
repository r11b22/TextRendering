#include "TextRendering/Text/Text.hpp"

#include "Renderer/RenderCommand.h"
#include "TextRendering/FontMetadata.hpp"
#include "TextRendering/GlyphMetadata.hpp"
#include "glm/ext/vector_float3.hpp"
#include <stdexcept>
#include <vector>

Text::Text(AssetReference<Font> font)
    :
    mText(""),
    mFont(font)
{}

void Text::setText(std::string text){
    mText = std::move(text);
}

const std::string& Text::getText() const {
    return mText;
}

MeshReference Text::createMesh(AssetManager& assetManager) {
    // Setup start vertex
    mVertices = {};

    // Setup start indices
    mIndices = {};

    mIndexOffset = 0;
    mVertexOffset = 0;
    mLastAdvance = 0;

    auto it = mText.begin();
    auto end = mText.end();
    for (unsigned char c : mText) {
        size_t character = c;
        createSingleCharacter(character, assetManager.getAsset(mFont));
    }


    if (!mMeshReference.isNoReference()){
        Mesh* mesh = assetManager.getAsset(mMeshReference);
        mesh->setVertices(mVertices);
        mesh->setIndices(mIndices);
    }else{
        Mesh mesh{"textMesh"};

        mesh.setVertices(mVertices);
        mesh.setIndices(mIndices);

        mMeshReference = assetManager.addAsset(std::move(mesh));
    }


    return mMeshReference;
}

void Text::setupUniforms(DrawCommand& command, const AssetManager& assetManager) const{
    const Font* font = assetManager.getAsset(mFont);

    command.staticUniforms.push_back({"uDistanceRange", font->getMetadata().getDistanceRange()});
    command.staticUniforms.push_back({"uDistanceRangeMiddle", font->getMetadata().getDistanceRangeMiddle()});
    command.staticUniforms.push_back({"uTextColor", mColor});
}

void Text::setColor(glm::vec3 color){
    mColor = std::move(color);
}

const glm::vec3& Text::getColor() const{
    return mColor;
}

void Text::createSingleCharacter(size_t character, Font* font){
    if (font == nullptr){
        throw std::runtime_error("No font provided to render text with!");
    }


    mVertexOffset += mLastAdvance;

    const GlyphMetadata& glyph = font->getMetadata().getGlyph(character);



    const GlyphVertices vertices = glyph.getPlaneBoundVertices();
    const GlyphUVs uvs = font->getMetadata().getGlyphUVs(character);
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
