#pragma once


#include "Asset/AssetReference.hpp"
#include "Defaults/Objects/Drawables/MeshObject.h"
#include "Renderer/RenderCommand.h"
#include "TextRendering/Font.hpp"
#include <cstddef>
#include <vector>
class TextObject : public MeshObject {
    private:
        std::string mText;
        AssetReference<Font> mFont;

        float mLastAdvance = 0;
        std::vector<float> mVertices;
        std::vector<unsigned int> mIndices;

        unsigned int mIndexOffset = 0;
        float mVertexOffset = 0;
    public:
        TextObject(std::string name, AssetReference<Font> font, std::string text);

        void onLoad() override;

        std::vector<RenderCommand> getRenderCommands() override;
    private:
        void createMesh();
        void createSingleCharacter(size_t character);
};
