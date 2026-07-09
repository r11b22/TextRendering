#pragma once


#include "TextRendering/GlyphMetadata.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/glm.hpp"

#include <optional>
#include <vector>

struct GlyphUVs {
    glm::vec2 topLeft;
    glm::vec2 bottomLeft;
    glm::vec2 topRight;
    glm::vec2 bottomRight;
};

class FontMetadata {
    private:
        glm::vec2 mDimensions;
        float mDistanceRange;
        float mDistanceRangeMiddle;

        std::vector<std::optional<GlyphMetadata>> mGlyphs;
    public:
        void setDimensions(glm::vec2 dimensions);

        void setDistanceRange(float distanceRange);
        float getDistanceRange() const;

        void setDistanceRangeMiddle(float distanceRangeMiddle);
        float getDistanceRangeMiddle() const;


        void addGlyph(GlyphMetadata glyph);
        const GlyphMetadata& getGlyph(size_t unicode) const;


        GlyphUVs getGlyphUVs(size_t unicode) const;
    private:
};
