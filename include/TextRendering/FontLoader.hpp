#pragma once

#include "Asset/AssetLoader.hpp"
#include "Asset/AssetManager.hpp"
#include "Serial/Json/Json.hpp"
#include "TextRendering/Font.hpp"
#include "TextRendering/FontMetadata.hpp"
#include <filesystem>
class FontLoader {
    private:
    public:
        Font readFiles(std::string name, std::filesystem::path atlas, std::filesystem::path metadata, AssetManager& assetManagers) const;
        FontMetadata metadataFromJson(const Json& json) const;
    private:

};

template<>
struct AssetLoadInfo<Font> {
    std::string name;
    std::filesystem::path atlasPath;
    std::filesystem::path metadataPath;
};

template<>
struct AssetLoader<Font> {
    static Font load(AssetLoadInfo<Font> asset, AssetManager& assetManager){
        FontLoader loader{};

        Font font = loader.readFiles(asset.name, asset.atlasPath, asset.metadataPath, assetManager);

        return std::move(font);
    }
};
