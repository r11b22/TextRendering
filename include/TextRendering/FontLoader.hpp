#pragma once

#include "Asset/AssetManager.hpp"
#include "Serial/Json/Json.hpp"
#include "TextRendering/Font.hpp"
#include "TextRendering/FontMetadata.hpp"
#include <filesystem>
class FontLoader {
    private:
    public:
        Font readFiles(std::filesystem::path atlas, std::filesystem::path metadata, AssetManager& assetManagers) const;
        FontMetadata metadataFromJson(const Json& json) const;
    private:

};
