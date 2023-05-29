#pragma once
#include "includes.hpp"

class AssetManager {
  public:
    UV uvForBlockType(BlockTypes type);
    Texture2D textureForName(const string& name);

    static std::shared_ptr<AssetManager> sharedState();

  private:
    std::unordered_map<string, Texture2D> m_textures;
};