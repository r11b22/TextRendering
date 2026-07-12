#pragma once


#include "Asset/AssetReference.hpp"
#include "Defaults/Objects/Drawables/MeshObject.h"
#include "TextRendering/Font.hpp"
#include "TextRendering/Text/Text.hpp"
#include "glm/ext/vector_float3.hpp"
#include <vector>

using namespace Spelt;

class TextUI : public Defaults::MeshObject {
    private:
        AssetReference<Font> mFont;

        Text mText;
    public:
        TextUI(std::string name, AssetReference<Font> font, std::string text);

        void setText(std::string text);

        void setColor(glm::vec3 color);
        const glm::vec3& getColor() const;

        void onLoad() override;

        std::vector<RenderPassCommands> getRenderCommands() override;
    private:
};
