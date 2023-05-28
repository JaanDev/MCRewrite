#include "AssetManager.hpp"

UV AssetManager::uvForBlockType(BlockTypes type) {
    switch (type) {
    case BlockTypes::Rock:
        return {1.f / 16.f, 0.f, 1.f / 16.f * 2.f, 1.f / 16.f};
    case BlockTypes::Grass:
        return {0.f, 0.f, 1.f / 16.f, 1.f / 16.f};
    default:
        return {0, 0, 0, 0};
    }
}

Texture2D AssetManager::textureForName(const string& name) {
    if (m_textures.count(name) > 0) {
        return m_textures[name];
    } else {
        auto tex = LoadTexture(name.c_str());
        m_textures.insert({name, tex});
        return tex;
    }
}

std::shared_ptr<AssetManager> AssetManager::sharedState() {
    static auto g_sharedAssetManager = std::make_shared<AssetManager>();
    return g_sharedAssetManager;
}
