#pragma once

#include "Asset/AssetManager.hpp"
#include "Asset/AssetReference.hpp"
#include "Mesh/MeshReference.hpp"
#include "Renderer/RenderCommand.h"
#include "TextRendering/Font.hpp"
#include "glm/ext/vector_float3.hpp"
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

        MeshReference mMeshReference;

        glm::vec3 mColor = glm::vec3{1.0f};
    public:
        Text(AssetReference<Font> font);


        void setText(std::string text);
        const std::string& getText() const;

        MeshReference createMesh(AssetManager& assetManager);
        void setupUniforms(DrawCommand& command, const AssetManager& assetManager) const;

        void setColor(glm::vec3 color);
        const glm::vec3& getColor() const;
    private:
        void createSingleCharacter(size_t character, Font* font);
};
