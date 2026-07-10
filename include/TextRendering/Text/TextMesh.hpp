#pragma once


#include "Asset/AssetReference.hpp"
#include "Defaults/Objects/Drawables/MeshObject.h"
#include "Renderer/RenderCommand.h"
#include "TextRendering/Font.hpp"
#include "TextRendering/Text/Text.hpp"
#include <cstddef>
#include <vector>
class TextMesh : public MeshObject {
    private:
        AssetReference<Font> mFont;

        Text mText;
    public:
        TextMesh(std::string name, AssetReference<Font> font, std::string text);

        void setText(std::string text);

        void onLoad() override;

        std::vector<RenderPassCommands> getRenderCommands() override;
    private:
};
