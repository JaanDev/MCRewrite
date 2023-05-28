#include "Chunk.hpp"

Chunk::Chunk(const ChunkPos& pos, Level* level) : m_pos(pos), m_level(level) {
    generate();
}

bool Chunk::isSolidTile(const BlockPos& pos) {
    return getBlock(pos) != BlockTypes::Air;
}

void Chunk::render() {
    DrawModel(m_model, {static_cast<float>(m_pos.x * chunkSize), 0, static_cast<float>(m_pos.z * chunkSize)}, 1.f, WHITE);
}

ChunkPos Chunk::getPos() {
    return m_pos;
}

uint32_t Chunk::getFaceCount() {
    uint32_t faces = 0;

    for (uint32_t x = 0; x < chunkSize; x++) {
        for (uint32_t y = 0; y < chunkHeight; y++) {
            for (uint32_t z = 0; z < chunkSize; z++) {
                BlockPos pos = {m_pos.x * chunkSize + x, y, m_pos.z * chunkSize + z};

                if (m_level->isSolidTile(pos)) {
                    if (!m_level->isSolidTile(pos + BlockPos {0, 1, 0})) // y+
                        faces++;

                    if (!m_level->isSolidTile(pos - BlockPos {0, 1, 0})) // y-
                        faces++;

                    if (!m_level->isSolidTile(pos + BlockPos {0, 0, 1})) // z+
                        faces++;

                    if (!m_level->isSolidTile(pos - BlockPos {0, 0, 1})) // z-
                        faces++;

                    if (!m_level->isSolidTile(pos + BlockPos {1, 0, 0})) // x+
                        faces++;

                    if (!m_level->isSolidTile(pos - BlockPos {1, 0, 0})) // x-
                        faces++;
                }
            }
        }
    }

    return faces;
}

void Chunk::unload() {
    UnloadModel(m_model);
}

Chunk::~Chunk() {
    unload();
}

void Chunk::generate() {
    for (uint8_t x = 0; x < chunkSize; x++) {
        for (uint16_t y = 0; y < chunkHeight; y++) {
            for (uint8_t z = 0; z < chunkSize; z++) {
                auto pos = BlockPos {x, y, z};

                setBlock(pos, BlockTypes::Air);

                // setBlock(pos, (BlockTypes)GetRandomValue(0, 1));

                if (y < surfaceLevel)
                    setBlock(pos, BlockTypes::Rock);
                else if (y == surfaceLevel)
                    setBlock(pos, BlockTypes::Grass);
            }
        }
    }
}

void Chunk::generateMesh() {
    // thx to https://github.com/raylib-extras/examples-cpp/blob/main/voxel_mesher/main.cpp
    Mesh m_mesh = {0};

    auto faceCount = getFaceCount();
    m_mesh.triangleCount = faceCount * 2;
    m_mesh.vertexCount = faceCount * 6;

    // logD("tris {} verts {} faces {}", m_mesh.triangleCount, m_mesh.vertexCount, faceCount);

    m_mesh.vertices = static_cast<float*>(MemAlloc(sizeof(float) * 3 * m_mesh.vertexCount));
    m_mesh.normals = static_cast<float*>(MemAlloc(sizeof(float) * 3 * m_mesh.vertexCount));
    // m_mesh.texcoords = nullptr;
    m_mesh.texcoords = static_cast<float*>(MemAlloc(sizeof(float) * 2 * m_mesh.vertexCount));
    // m_mesh.colors = static_cast<uint8_t*>(MemAlloc(sizeof(uint8_t) * 4 * m_mesh.vertexCount));
    m_mesh.colors = nullptr;

    m_mesh.animNormals = nullptr;
    m_mesh.animVertices = nullptr;
    m_mesh.boneIds = nullptr;
    m_mesh.boneWeights = nullptr;
    m_mesh.tangents = nullptr;
    m_mesh.indices = nullptr;
    m_mesh.texcoords2 = nullptr;

    uint32_t index = 0;

#define ADD_VERTEX(offX, offY, offZ, texU, texV)                                                                                           \
    newPos = pos + BlockPos {offX, offY, offZ};                                                                                            \
                                                                                                                                           \
    m_mesh.vertices[index * 3] = newPos.x;                                                                                                 \
    m_mesh.vertices[index * 3 + 1] = newPos.y;                                                                                             \
    m_mesh.vertices[index * 3 + 2] = newPos.z;                                                                                             \
                                                                                                                                           \
    m_mesh.normals[index * 3] = normal.x;                                                                                                  \
    m_mesh.normals[index * 3 + 1] = normal.y;                                                                                              \
    m_mesh.normals[index * 3 + 2] = normal.z;                                                                                              \
                                                                                                                                           \
    /*m_mesh.colors[index * 4] = col.x; */                                                                                                 \
    /*m_mesh.colors[index * 4 + 1] = col.y; */                                                                                             \
    /*m_mesh.colors[index * 4 + 2] = col.z; */                                                                                             \
    /*m_mesh.colors[index * 4 + 3] = col.w; */                                                                                             \
                                                                                                                                           \
    m_mesh.texcoords[index * 2] = texU;                                                                                                    \
    m_mesh.texcoords[index * 2 + 1] = texV;                                                                                                \
    index++;

    for (uint8_t x = 0; x < chunkSize; x++) {
        for (uint16_t y = 0; y < chunkHeight; y++) {
            for (uint8_t z = 0; z < chunkSize; z++) {
                auto globalPos = BlockPos {m_pos.x * chunkSize + x, y, m_pos.z * chunkSize + z};
                auto pos = BlockPos {x, y, z};

                if (m_level->isSolidTile(globalPos)) {
                    Vector3 normal;
                    // Vector4 col;
                    BlockPos newPos;
                    auto uv = AssetManager::sharedState()->uvForBlockType(getBlock(pos));
                    // auto uv = UV {0, 0, 1, 1};

                    if (!m_level->isSolidTile(globalPos + BlockPos {0, 1, 0})) { // y+
                        normal = {0, 1, 0};
                        // col = {0, 0, 0, 255};
                        ADD_VERTEX(0, 1, 0, uv.startX, uv.startY)
                        ADD_VERTEX(0, 1, 1, uv.startX, uv.endY)
                        ADD_VERTEX(1, 1, 0, uv.endX, uv.startY)

                        ADD_VERTEX(0, 1, 1, uv.startX, uv.endY)
                        ADD_VERTEX(1, 1, 1, uv.endX, uv.endY)
                        ADD_VERTEX(1, 1, 0, uv.endX, uv.startY)
                    }

                    if (!m_level->isSolidTile(globalPos - BlockPos {0, 1, 0})) { // y-
                        normal = {0, -1, 0};
                        ADD_VERTEX(1, 0, 0, uv.endX, uv.startY)
                        ADD_VERTEX(0, 0, 1, uv.startX, uv.endY)
                        ADD_VERTEX(0, 0, 0, uv.startX, uv.startY)

                        ADD_VERTEX(1, 0, 0, uv.endX, uv.startY)
                        ADD_VERTEX(1, 0, 1, uv.endX, uv.endY)
                        ADD_VERTEX(0, 0, 1, uv.startX, uv.endY)
                    }

                    if (!m_level->isSolidTile(globalPos + BlockPos {0, 0, 1})) { // z+
                        normal = {0, 0, 1};
                        ADD_VERTEX(0, 0, 1, uv.startX, uv.endY)
                        ADD_VERTEX(1, 0, 1, uv.endX, uv.endY)
                        ADD_VERTEX(1, 1, 1, uv.endX, uv.startY)

                        ADD_VERTEX(0, 0, 1, uv.startX, uv.endY)
                        ADD_VERTEX(1, 1, 1, uv.endX, uv.startY)
                        ADD_VERTEX(0, 1, 1, uv.startX, uv.startY)
                    }

                    if (!m_level->isSolidTile(globalPos - BlockPos {0, 0, 1})) { // z-
                        normal = {0, 0, -1};
                        ADD_VERTEX(1, 1, 0, uv.startX, uv.startY)
                        ADD_VERTEX(1, 0, 0, uv.startX, uv.endY)
                        ADD_VERTEX(0, 0, 0, uv.endX, uv.endY)

                        ADD_VERTEX(0, 1, 0, uv.endX, uv.startY)
                        ADD_VERTEX(1, 1, 0, uv.startX, uv.startY)
                        ADD_VERTEX(0, 0, 0, uv.endX, uv.endY)
                    }

                    if (!m_level->isSolidTile(globalPos + BlockPos {1, 0, 0})) { // x+
                        normal = {1, 0, 0};
                        ADD_VERTEX(1, 0, 0, uv.endX, uv.endY)
                        ADD_VERTEX(1, 1, 0, uv.endX, uv.startY)
                        ADD_VERTEX(1, 0, 1, uv.startX, uv.endY)

                        ADD_VERTEX(1, 1, 0, uv.endX, uv.startY)
                        ADD_VERTEX(1, 1, 1, uv.startX, uv.startY)
                        ADD_VERTEX(1, 0, 1, uv.startX, uv.endY)
                    }

                    if (!m_level->isSolidTile(globalPos - BlockPos {1, 0, 0})) { // x-
                        normal = {-1, 0, 0};
                        ADD_VERTEX(0, 0, 0, uv.startX, uv.endY)
                        ADD_VERTEX(0, 0, 1, uv.endX, uv.endY)
                        ADD_VERTEX(0, 1, 1, uv.endX, uv.startY)

                        ADD_VERTEX(0, 0, 0, uv.startX, uv.endY)
                        ADD_VERTEX(0, 1, 1, uv.endX, uv.startY)
                        ADD_VERTEX(0, 1, 0, uv.startX, uv.startY)
                    }
                }
            }
        }
    }

    // logD("index {}", index);

    UploadMesh(&m_mesh, false);

    // Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    // Texture2D texture = LoadTextureFromImage(checked);
    // UnloadImage(checked);

    // auto texture = LoadRenderTexture(32, 16);
    // BeginTextureMode(texture);
    // ClearBackground(BLANK);
    // DrawRectangle(0, 0, 16, 16, GREEN);
    // DrawRectangle(16, 0, 16, 16, GRAY);
    // EndTextureMode();

    // auto texture = LoadTexture("assets/arrow.png");

    // auto texture = AssetManager::sharedState()->textureForName("assets/gradient.png");
    auto texture = AssetManager::sharedState()->textureForName("assets/terrain.png");

    m_model = LoadModelFromMesh(m_mesh);
    m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

BlockTypes Chunk::getBlock(const BlockPos& pos) {
    if (pos.x >= chunkSize || pos.x < 0 || pos.y >= chunkHeight || pos.y < 0 || pos.z >= chunkSize || pos.z < 0)
        return BlockTypes::Air;

    return m_data[pos.x][pos.y][pos.z];
}

void Chunk::setBlock(const BlockPos& pos, BlockTypes type) {
    if (pos.x >= chunkSize || pos.x < 0 || pos.y >= chunkHeight || pos.y < 0 || pos.z >= chunkSize || pos.z < 0)
        return;

    m_data[pos.x][pos.y][pos.z] = type;
}
