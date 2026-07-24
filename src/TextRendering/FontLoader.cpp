#include "TextRendering/FontLoader.hpp"
#include "Asset/AssetLoader.hpp"
#include "Error/Option.hpp"
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

    // TODO use returned result
    mMetadataJson = std::move(loader.createJson().value("Font metadata contained invalid Json"));


}

void FontLoader::readRaw(EmbeddedAsset atlas, EmbeddedAsset metadata, AssetManager& assetManagers) {
    AssetLoadInfo<Texture> atlasLoadInfo = AssetLoadInfo<Texture>::FromEmbedded("fontAtlas", atlas, true);

    mTextureReference = assetManagers.loadAsset(atlasLoadInfo);

    Texture* texture = assetManagers.getAsset(mTextureReference);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    JsonLoader loader{};

    loader.readRaw(metadata);

    mMetadataJson = std::move(loader.createJson().value("Font metadata contained invalid Json"));
}

Font FontLoader::createFont(std::string name){
    Font font{std::move(name), metadataFromJson(mMetadataJson), mTextureReference};

    return font;
}


FontMetadata FontLoader::metadataFromJson(const Json& json) const{
    FontMetadata metadata{};

    Json atlas = json.getObject("atlas").value("Could not find atlas in font metadata");

    int width = atlas.getNumber("width").value("Could not find atlas.width in font metadata");
    int height = atlas.getNumber("height").value("Could not find atlas.height in font metadata");

    metadata.setDimensions({width, height});

    float distanceRange = atlas.getNumber("distanceRange").value("Could not find atlas.distanceRange in font metadata");
    metadata.setDistanceRange(distanceRange);
    float distanceRangeMiddle = atlas.getNumber("distanceRangeMiddle").value("Could not find atlas.distanceRangeMiddle in font metadata");
    metadata.setDistanceRangeMiddle(distanceRangeMiddle);

    JsonArray glyphArray = json.getArray("glyphs").value("Could not find glyphs in font metadata");
    for (int i = 0; i < glyphArray.size(); i++){
        GlyphMetadata glyphMetadata{};


        Json glyphJson = glyphArray.getObject(i).value();
        size_t unicode = static_cast<size_t>(glyphJson.getNumber("unicode").value(std::format("Could not find glyphs[{}].unicode in font metadata", i)));
        double advance = glyphJson.getNumber("advance").value(std::format("Could not find glyphs[{}].advance in font metadata", i));

        glyphMetadata.setUnicode(unicode);
        glyphMetadata.setAdvance(advance);

        Option<const Json&> planeBoundsO = glyphJson.getObject("planeBounds");

        if (planeBoundsO.isValue()){
            Json planeBounds = planeBoundsO.value();
            glm::vec4 bounds{
                planeBounds.getNumber("left").value(std::format("Could not find glyphs[{}].planeBounds.left in font metadata", i)),
                planeBounds.getNumber("bottom").value(std::format("Could not find glyphs[{}].planeBounds.bottom in font metadata", i)),
                planeBounds.getNumber("right").value(std::format("Could not find glyphs[{}].planeBounds.right in font metadata", i)),
                planeBounds.getNumber("top").value(std::format("Could not find glyphs[{}].planeBounds.top in font metadata", i))
            };
            glyphMetadata.setPlaneBounds(bounds);
        }

        Option<const Json&> atlasBoundsO = glyphJson.getObject("atlasBounds");

        if (atlasBoundsO.isValue()){
            Json atlasBounds = atlasBoundsO.value();
            glm::vec4 bounds{
                atlasBounds.getNumber("left").value(std::format("Could not find glyphs[{}].atlasBounds.left in font metadata", i)),
                atlasBounds.getNumber("bottom").value(std::format("Could not find glyphs[{}].atlasBounds.bottom in font metadata", i)),
                atlasBounds.getNumber("right").value(std::format("Could not find glyphs[{}].atlasBounds.right in font metadata", i)),
                atlasBounds.getNumber("top").value(std::format("Could not find glyphs[{}].atlasBounds.top in font metadata", i))
            };            glyphMetadata.setAtlasBounds(bounds);
        }

        metadata.addGlyph(std::move(glyphMetadata));
    }

    return metadata;
}
