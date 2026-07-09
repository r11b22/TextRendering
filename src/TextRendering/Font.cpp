#include "TextRendering/Font.hpp"
#include "TextRendering/FontMetadata.hpp"
#include "Texture/TextureReference.hpp"



Font::Font(std::string name, FontMetadata metadata, TextureReference atlas)
    : Asset(std::move(name)), mMetadata(metadata), mAtlas(atlas)
{}


TextureReference Font::getAtlas() const {
    return mAtlas;
}

const FontMetadata& Font::getMetadata() const {
    return mMetadata;
}
