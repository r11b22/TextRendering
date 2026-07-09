#include "TextRendering/Font.hpp"
#include "TextRendering/FontMetadata.hpp"
#include "Texture/TextureReference.hpp"



Font::Font(FontMetadata metadata, TextureReference atlas)
    : mMetadata(metadata), mAtlas(atlas)
{}


TextureReference Font::getAtlas() const {
    return mAtlas;
}

const FontMetadata& Font::getMetadata() const {
    return mMetadata;
}
