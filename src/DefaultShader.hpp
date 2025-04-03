#pragma once
#include <string_view>

inline std::string_view vertexShader = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aShadow;

out vec3 fragColor;
out float fragAlpha;
out vec2 fragUV;
out vec3 fragPos;
out float fragShadow;
out float fogFactor;

uniform mat4 projection;
uniform mat4 view;
uniform float alpha;

void main() {
    vec4 worldPos = vec4(aPos, 1.0);
    gl_Position = projection * view * worldPos;

    vec4 viewPos = view * worldPos;
    float distance = abs(viewPos.z);
    fogFactor = exp(-0.2 * distance);

    fragPos = aPos;
    fragColor = aColor;
    fragUV = aUV;
    fragAlpha = alpha;
    fragShadow = aShadow;
})";

inline std::string_view fragmentShader = R"(#version 330 core
in vec3 fragColor;
in float fragAlpha;
in vec2 fragUV;

in float fragShadow;
in float fogFactor;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 fogColor;

void main() {
    vec4 color = texture(tex, fragUV) * vec4(fragColor, fragAlpha);

    if (fragShadow != 0.0) {
        color = fogFactor * color + (1.0 - fogFactor) * vec4(fogColor, 1.0);
    }

    FragColor = color;
})";
