#pragma once

#include "Asset/Asset.hpp"
#include "Asset/AssetReference.hpp"
#include "TextRendering/FontMetadata.hpp"
#include "Texture/TextureReference.hpp"

using namespace Spelt;


class Font : public Asset{
    private:
        FontMetadata mMetadata;
        TextureReference mAtlas;
    public:
        Font(std::string name, FontMetadata metadata, TextureReference atlas);

        TextureReference getAtlas() const;

        const FontMetadata& getMetadata() const;
    private:
};
