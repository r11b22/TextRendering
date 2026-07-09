#version 430 core
layout(location = 0) out vec4 FragColor;

struct MaterialProperties {
    int materialType;
    float diffuse;
    float specular;
    float shininess;
};

uniform MaterialProperties uMaterialProperties;

uniform sampler2D uTexture;

uniform float uDistanceRange;
uniform float uDistanceRangeMiddle;

uniform vec3 uTextColor;

uniform vec3 uCameraPos; // camera location
in vec3 FragPos; // pixel position
in vec3 Normal;
in vec2 UV;

float getDistance(vec4 color) {
    const float signedSDF = color.r - 0.5;

    const float pixelDistance = (signedSDF * uDistanceRange) + uDistanceRangeMiddle;

    return pixelDistance;
}

void main()
{
    const vec4 texColor = texture(uTexture, UV);
    const float distance = getDistance(texColor);

    float width = fwidth(distance);

    float alpha = smoothstep(-width, width, distance);


    vec4 outColor = vec4(uTextColor, alpha);

    if(outColor.a < 0.01){
        discard;
    }

    FragColor = outColor;
}
