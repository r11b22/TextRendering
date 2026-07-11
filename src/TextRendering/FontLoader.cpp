#include "TextRendering/FontLoader.hpp"
#include "Asset/AssetLoader.hpp"
#include "Serial/Json/JsonLoader.hpp"
#include "TextRendering/FontMetadata.hpp"
#include "TextRendering/GlyphMetadata.hpp"
#include "Texture/Texture.h"
#include "Texture/TextureAsset.hpp"
#include "Texture/TextureLoader.h"
#include "Texture/TextureReference.hpp"
#include <cstddef>
#include <iostream>
#include <optional>

void FontLoader::readFiles(std::filesystem::path atlas, std::filesystem::path metadata, AssetManager& assetManagers) {
    AssetLoadInfo<Texture> atlasLoadInfo = AssetLoadInfo<Texture>::FromPath("fontAtlas", atlas, true);

    mTextureReference = assetManagers.loadAsset(atlasLoadInfo);

    Texture* texture = assetManagers.getAsset(mTextureReference);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    JsonLoader loader{};
    loader.readFile(metadata);

    mMetadataJson = std::move(loader.createJson());


}

void FontLoader::readRaw(EmbeddedAsset atlas, EmbeddedAsset metadata, AssetManager& assetManagers) {
    AssetLoadInfo<Texture> atlasLoadInfo = AssetLoadInfo<Texture>::FromEmbedded("fontAtlas", atlas, true);

    mTextureReference = assetManagers.loadAsset(atlasLoadInfo);

    Texture* texture = assetManagers.getAsset(mTextureReference);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    JsonLoader loader{};

    loader.readRaw(metadata);

    mMetadataJson = std::move(loader.createJson());
}

Font FontLoader::createFont(std::string name){
    Font font{std::move(name), metadataFromJson(mMetadataJson), mTextureReference};

    return font;
}


FontMetadata FontLoader::metadataFromJson(const Json& json) const{
    FontMetadata metadata{};

    Json atlas = json.getObject("atlas");

    int width = atlas.getNumber("width");
    int height = atlas.getNumber("height");

    metadata.setDimensions({width, height});

    float distanceRange = atlas.getNumber("distanceRange");
    metadata.setDistanceRange(distanceRange);
    float distanceRangeMiddle = atlas.getNumber("distanceRangeMiddle");
    metadata.setDistanceRangeMiddle(distanceRangeMiddle);

    JsonArray glyphArray = json.getArray("glyphs");
    for (int i = 0; i < glyphArray.size(); i++){
        GlyphMetadata glyphMetadata{};


        Json glyphJson = glyphArray.getObject(i);
        size_t unicode = static_cast<size_t>(glyphJson.getNumber("unicode"));
        double advance = glyphJson.getNumber("advance");

        glyphMetadata.setUnicode(unicode);
        glyphMetadata.setAdvance(advance);

        std::optional<Json> planeBoundsO = glyphJson.tryGetObject("planeBounds");

        if (planeBoundsO.has_value()){
            Json planeBounds = planeBoundsO.value();
            glm::vec4 bounds{planeBounds.getNumber("left"), planeBounds.getNumber("bottom"), planeBounds.getNumber("right"), planeBounds.getNumber("top")};
            glyphMetadata.setPlaneBounds(bounds);
        }

        std::optional<Json> atlasBoundsO = glyphJson.tryGetObject("atlasBounds");

        if (atlasBoundsO.has_value()){
            Json atlasBounds = atlasBoundsO.value();
            glm::vec4 bounds{atlasBounds.getNumber("left"), atlasBounds.getNumber("bottom"), atlasBounds.getNumber("right"), atlasBounds.getNumber("top")};
            glyphMetadata.setAtlasBounds(bounds);
        }

        metadata.addGlyph(std::move(glyphMetadata));
    }

    return metadata;
}
