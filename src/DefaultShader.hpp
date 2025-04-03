#pragma once
#include <string_view>

inline std::string_view vertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in int aShadow;

out vec3 fragColor;
out vec2 fragUV;
out vec3 fragPos;
flat out int fragShadow;

out float fogFactor;

uniform mat4 projection;
uniform mat4 view;

void main() {
    vec4 worldPos = vec4(aPos, 1.0);
    gl_Position = projection * view * worldPos;

    vec4 viewPos = view * worldPos;
    float distance = abs(viewPos.z);
    fogFactor = exp(-0.2 * distance);

    fragPos = aPos;
    fragColor = aColor;
    fragUV = aUV;
    fragShadow = aShadow;
})";

inline std::string_view fragmentShader = R"(
#version 330 core
in vec3 fragColor;
in vec2 fragUV;
flat in int fragShadow;

in float fogFactor;

out vec4 FragColor;

uniform sampler2D tex;
uniform float alpha;
uniform vec3 uColor;
uniform bool useUColor;
uniform bool useTexture;

void main() {
    const vec3 fogColor = vec3(14.f / 255.f, 11.f / 255.f, 10.f / 255.f);

    vec4 color = vec4(fragColor, alpha);

    if (useUColor) {
        color = vec4(uColor, alpha);
    }

    if (useTexture) {
        color *= texture(tex, fragUV);
    }

    if (fragShadow != 0) {
        color = fogFactor * color + (1.0 - fogFactor) * vec4(fogColor, 1.0);
    }

    FragColor = color;
})";
