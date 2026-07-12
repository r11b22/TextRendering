#pragma once

#include "Asset/AssetLoader.hpp"
#include "Asset/AssetManager.hpp"
#include "Serial/Json/Json.hpp"
#include "TextRendering/Font.hpp"
#include "TextRendering/FontMetadata.hpp"
#include <algorithm>
#include <filesystem>
#include "Asset/EmbeddedAsset.hpp"
#include "Texture/TextureReference.hpp"

class FontLoader {
    private:
        TextureReference mTextureReference;
        Json mMetadataJson;
    public:
        void readFiles(std::filesystem::path atlas, std::filesystem::path metadata, AssetManager& assetManagers);
        void readRaw(EmbeddedAsset atlas, EmbeddedAsset metadata, AssetManager& assetManagers);

        Font createFont(std::string name);
        FontMetadata metadataFromJson(const Json& json) const;
    private:

};



namespace Spelt {
    struct FontPathLoadSource {
        std::filesystem::path atlasPath;
        std::filesystem::path metadataPath;
    };

    struct FontRawLoadSource {
        EmbeddedAsset atlas;
        EmbeddedAsset metadata;
    };

    template<>
    struct AssetLoadInfo<Font> {
        std::string name;

        std::variant<FontPathLoadSource, FontRawLoadSource> source;

        static AssetLoadInfo FromPath(std::string name, std::filesystem::path atlasPath, std::filesystem::path metadataPath) {
            return AssetLoadInfo(
                std::move(name),
                FontPathLoadSource{std::move(atlasPath), std::move(metadataPath)}
            );
        }

        static AssetLoadInfo FromEmbedded(std::string name, EmbeddedAsset atlas, EmbeddedAsset metadata) {
            return AssetLoadInfo(
                std::move(name),
                FontRawLoadSource{std::move(atlas), std::move(metadata)}
            );
        }

    private:
        /*
         * Hidden constructor
         */
        AssetLoadInfo(std::string name, std::variant<FontPathLoadSource, FontRawLoadSource> src)
            : name(std::move(name)), source(std::move(src)) {}
    };

    template<>
    struct AssetLoader<Font> {
        static Font load(AssetLoadInfo<Font> asset, AssetManager& assetManager){
            ::FontLoader loader{};

            if (std::holds_alternative<FontPathLoadSource>(asset.source)) {
                const auto& pathSrc = std::get<FontPathLoadSource>(asset.source);
                loader.readFiles(pathSrc.atlasPath, pathSrc.metadataPath, assetManager);
            }
            else if (std::holds_alternative<FontRawLoadSource>(asset.source)) {
                const auto& rawSrc = std::get<FontRawLoadSource>(asset.source);
                loader.readRaw(rawSrc.atlas, rawSrc.metadata, assetManager);
            }



            return std::move(loader.createFont(asset.name));
        }
    };
};
