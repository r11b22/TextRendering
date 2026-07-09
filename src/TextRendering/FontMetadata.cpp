#include "TextRendering/FontMetadata.hpp"
#include "TextRendering/GlyphMetadata.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cstddef>
#include <format>
#include <optional>
#include <stdexcept>


void FontMetadata::setDimensions(glm::vec2 dimensions){
    mDimensions = dimensions;
}

void FontMetadata::setDistanceRange(float distanceRange){
    mDistanceRange = distanceRange;
}
float FontMetadata::getDistanceRange() const{
    return mDistanceRange;
}

void FontMetadata::setDistanceRangeMiddle(float distanceRangeMiddle){
    mDistanceRangeMiddle = distanceRangeMiddle;
}
float FontMetadata::getDistanceRangeMiddle() const {
    return mDistanceRangeMiddle;
}

void FontMetadata::addGlyph(GlyphMetadata glyph){
    size_t idx = glyph.getUnicode();

    if (idx >= mGlyphs.size()){
        mGlyphs.resize(idx+1);
    }

    mGlyphs[idx] = std::move(glyph);
}

const GlyphMetadata& FontMetadata::getGlyph(size_t unicode) const{
    const std::optional<GlyphMetadata>& oData = mGlyphs[unicode];

    if(oData.has_value()){
        return oData.value();
    }else{
        throw std::out_of_range(std::format("No glyph with unicode: {}, found", unicode));
    }

}

GlyphUVs FontMetadata::getGlyphUVs(size_t unicode) const{
    float width = mDimensions.x;
    float height = mDimensions.y;

    const GlyphMetadata& glyph = getGlyph(unicode);

    glm::vec4 atlasBounds = glyph.getAtlasBounds();

    float normalizedLeft = atlasBounds.x / width;
    float normalizedBottom = atlasBounds.y / height;

    float normalizedRight = atlasBounds.z / width;
    float normalizedTop = atlasBounds.w / height;

    GlyphUVs uvs{};

    uvs.topLeft = glm::vec2{normalizedLeft, normalizedTop};
    uvs.bottomLeft = glm::vec2{normalizedLeft, normalizedBottom};
    uvs.topRight = glm::vec2{normalizedRight, normalizedTop};
    uvs.bottomRight = glm::vec2{normalizedRight, normalizedBottom};

    return uvs;
}
