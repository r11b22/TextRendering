#pragma once

#include "glm/glm.hpp"

struct GlyphVertices {
    glm::vec2 topLeft;
    glm::vec2 bottomLeft;
    glm::vec2 topRight;
    glm::vec2 bottomRight;
};

class GlyphMetadata {
    private:
        size_t mUnicode;
        double mAdvance;
        glm::vec4 mPlaneBounds;
        glm::vec4 mAtlasBounds;
    public:

        void setUnicode(uint64_t unicode);
        uint64_t getUnicode() const;

        void setAdvance(double advance);
        double getAdvance() const;

        void setPlaneBounds(glm::vec4 bounds);
        glm::vec4 getPlaneBounds() const;
        GlyphVertices getPlaneBoundVertices() const;

        void setAtlasBounds(glm::vec4 bounds);
        glm::vec4 getAtlasBounds() const;
    private:
};
