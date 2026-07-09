#pragma once

#include "TextRendering/FontMetadata.hpp"
#include "Texture/TextureReference.hpp"
class Font {
    private:
        FontMetadata mMetadata;
        TextureReference mAtlas;
    public:
        Font(FontMetadata metadata, TextureReference atlas);

        TextureReference getAtlas() const;

        const FontMetadata& getMetadata() const;
    private:
};
