#pragma once

#include "Asset/AssetManager.hpp"
#include "Asset/AssetReference.hpp"
#include "Mesh/MeshReference.hpp"
#include "Renderer/RenderCommand.h"
#include "TextRendering/Font.hpp"
#include <string>
#include <vector>

class Text {
    private:
        std::string mText;

        AssetReference<Font> mFont;

        float mLastAdvance = 0;
        std::vector<float> mVertices;
        std::vector<unsigned int> mIndices;

        unsigned int mIndexOffset = 0;
        float mVertexOffset = 0;
    public:
        Text(std::string text, AssetReference<Font> font);

        MeshReference createMesh(AssetManager& assetManager);
        void setupUniforms(DrawCommand& command, const AssetManager& assetManager) const;
    private:
        void createSingleCharacter(size_t character, Font* font);
};
