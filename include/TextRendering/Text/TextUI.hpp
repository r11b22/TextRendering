#pragma once


#include "Asset/AssetReference.hpp"
#include "Defaults/Objects/Drawables/MeshObject.h"
#include "TextRendering/Font.hpp"
#include "TextRendering/Text/Text.hpp"
#include <vector>

class TextUI : public MeshObject {
    private:
        AssetReference<Font> mFont;

        Text mText;
    public:
        TextUI(std::string name, AssetReference<Font> font, std::string text);

        void onLoad() override;

        std::vector<RenderPassCommands> getRenderCommands() override;
    private:
};
