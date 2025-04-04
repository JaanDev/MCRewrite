#pragma once
#include <string>

namespace Textures {
    int loadTexture(const std::string& resourceName, int mode);
    void bind(int id);
}; // namespace Textures