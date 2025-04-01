#pragma once
#include <string_view>

inline std::string_view vertexShader = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec3 fragColor;
out vec2 fragUV;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    fragColor = aColor;
    fragUV = aUV;
})";

inline std::string_view fragmentShader = R"(#version 330 core
in vec3 fragColor;
in vec2 fragUV;
out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex, fragUV) * vec4(fragColor, 1.0);
})";