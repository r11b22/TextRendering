
#include "TextRendering/GlyphMetadata.hpp"

void GlyphMetadata::setUnicode(uint64_t unicode){
    mUnicode = unicode;
}
uint64_t GlyphMetadata::getUnicode() const{
    return mUnicode;
}

void GlyphMetadata::setAdvance(double advance){
    mAdvance = advance;
}
double GlyphMetadata::getAdvance() const{
    return mAdvance;
}

void GlyphMetadata::setPlaneBounds(glm::vec4 bounds){
    mPlaneBounds = bounds;
}
glm::vec4 GlyphMetadata::getPlaneBounds() const{
    return mPlaneBounds;
}
GlyphVertices GlyphMetadata::getPlaneBoundVertices() const {
    GlyphVertices vertices{};
    glm::vec4 atlasBounds = getPlaneBounds();

    float left = atlasBounds.x;
    float bottom = atlasBounds.y;

    float right = atlasBounds.z;
    float top = atlasBounds.w;

    vertices.topLeft = glm::vec2{left, top};
    vertices.bottomLeft = glm::vec2{left, bottom};
    vertices.topRight = glm::vec2{right, top};
    vertices.bottomRight = glm::vec2{right, bottom};

    return vertices;
}

void GlyphMetadata::setAtlasBounds(glm::vec4 bounds){
    mAtlasBounds = bounds;
}
glm::vec4 GlyphMetadata::getAtlasBounds() const{
    return mAtlasBounds;
}
