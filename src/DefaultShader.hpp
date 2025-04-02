#pragma once
#include <string_view>

inline std::string_view vertexShader = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec3 fragColor;
out float fragAlpha;
out vec2 fragUV;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform float alpha;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    FragPos = aPos;
    fragColor = aColor;
    fragUV = aUV;
    fragAlpha = alpha;
})";

inline std::string_view fragmentShader = R"(#version 330 core
in vec3 fragColor;
in float fragAlpha;
in vec2 fragUV;

in vec3 FragPos;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 fogColor;
uniform vec3 cameraPos;
uniform float fogDensity = 0.125;

void main() {
    float brightness = (fragColor.r + fragColor.g + fragColor.b) / 3.0;
    float distance = length(cameraPos - FragPos);

    float fogFactor = 1.0 - exp(-pow(distance * fogDensity, 2));
    float lightFactor = smoothstep(0.8, 0.9, brightness); 
    vec3 finalColor = mix(fragColor, fogColor, fogFactor * (1.0 - lightFactor));

    FragColor = texture(tex, fragUV) * vec4(finalColor, fragAlpha);
})";
