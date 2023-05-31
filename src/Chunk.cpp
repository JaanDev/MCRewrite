#include "Chunk.hpp"

Chunk::Chunk(const ChunkPos& pos, Level* level) : m_pos(pos), m_level(level), m_built(false) {
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
    TIME_MEASURE_BEGIN(GetFaceCount)
    uint32_t faces = 0;

    auto chunkXP = m_level->getChunk(ChunkPos {m_pos.x + 1, m_pos.z}); // x+
    auto chunkXM = m_level->getChunk(ChunkPos {m_pos.x - 1, m_pos.z}); // x-
    auto chunkZP = m_level->getChunk(ChunkPos {m_pos.x, m_pos.z + 1}); // z+
    auto chunkZM = m_level->getChunk(ChunkPos {m_pos.x, m_pos.z - 1}); // z-

    for (uint8_t x = 0; x < chunkSize; x++) {
        for (uint16_t y = 0; y < chunkHeight; y++) {
            for (uint8_t z = 0; z < chunkSize; z++) {
                auto localPos = BlockPos {x, y, z};
                auto globalPos = BlockPos {m_pos.x * chunkSize + x, y, m_pos.z * chunkSize + z};

                if (isSolidTile(localPos)) {
                    // y+
                    if (!isSolidTile({x, y + 1, z}))
                        faces++;

                    // y-
                    if (!isSolidTile({x, y - 1, z}))
                        faces++;

                    // z-
                    if (z == 0) {
                        if (m_pos.z == 0) // the block there will be air
                            faces++;
                        else if (!chunkZM->isSolidTile({x, y, chunkSize - 1}))
                            faces++;
                    } else {
                        if (!isSolidTile({x, y, z - 1}))
                            faces++;
                    }

                    // z+
                    if (z == chunkSize - 1) {
                        if (m_pos.z == chunksCount - 1) // the block there will be air
                            faces++;
                        else if (!chunkZP->isSolidTile({x, y, 0}))
                            faces++;
                    } else {
                        if (!isSolidTile({x, y, z + 1}))
                            faces++;
                    }

                    // x-
                    if (x == 0) {
                        if (m_pos.x == 0) // the block there will be air
                            faces++;
                        else if (!chunkXM->isSolidTile({chunkSize - 1, y, z}))
                            faces++;
                    } else {
                        if (!isSolidTile({x - 1, y, z}))
                            faces++;
                    }

                    // x+
                    if (x == chunkSize - 1) {
                        if (m_pos.x == chunksCount - 1) // the block there will be air
                            faces++;
                        else if (!chunkXP->isSolidTile({0, y, z}))
                            faces++;
                    } else {
                        if (!isSolidTile({x + 1, y, z}))
                            faces++;
                    }
                }
            }
        }
    }
    TIME_MEASURE_END(GetFaceCount)
    TIME_MEASURE_DBG(GetFaceCount)

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

                // setBlock(pos, (BlockTypes)GetRandomValue(0, 2));

                setBlock(pos, BlockTypes::Air);

                if (y < surfaceLevel)
                    setBlock(pos, BlockTypes::Rock);
                else if (y == surfaceLevel)
                    setBlock(pos, BlockTypes::Grass);
            }
        }
    }
}

void Chunk::generateMesh() {
    TIME_MEASURE_BEGIN(GenMeshTotal)

    if (m_built)
        unload();

    // thx to https://github.com/raylib-extras/examples-cpp/blob/main/voxel_mesher/main.cpp
    TIME_MEASURE_BEGIN(GenMeshStart)
    Mesh m_mesh = {0};

    auto faceCount = getFaceCount();
    m_mesh.triangleCount = faceCount * 2;
    m_mesh.vertexCount = faceCount * 6;

    TIME_MEASURE_BEGIN(GenMeshStartArrays)
    m_mesh.vertices = static_cast<float*>(MemAlloc(sizeof(float) * 3 * m_mesh.vertexCount));
    m_mesh.normals = static_cast<float*>(MemAlloc(sizeof(float) * 3 * m_mesh.vertexCount));
    m_mesh.texcoords = static_cast<float*>(MemAlloc(sizeof(float) * 2 * m_mesh.vertexCount));
    m_mesh.colors = static_cast<uint8_t*>(MemAlloc(sizeof(uint8_t) * 4 * m_mesh.vertexCount));
    TIME_MEASURE_END(GenMeshStartArrays)
    TIME_MEASURE_DBG(GenMeshStartArrays)

    m_mesh.animNormals = nullptr;
    m_mesh.animVertices = nullptr;
    m_mesh.boneIds = nullptr;
    m_mesh.boneWeights = nullptr;
    m_mesh.tangents = nullptr;
    m_mesh.indices = nullptr;
    m_mesh.texcoords2 = nullptr;
    TIME_MEASURE_END(GenMeshStart)
    TIME_MEASURE_DBG(GenMeshStart)

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
    m_mesh.colors[index * 4] = col.r;                                                                                                      \
    m_mesh.colors[index * 4 + 1] = col.g;                                                                                                  \
    m_mesh.colors[index * 4 + 2] = col.b;                                                                                                  \
    m_mesh.colors[index * 4 + 3] = col.a;                                                                                                  \
                                                                                                                                           \
    m_mesh.texcoords[index * 2] = texU;                                                                                                    \
    m_mesh.texcoords[index * 2 + 1] = texV;                                                                                                \
    index++;

#define CALC_COL(offX, offY, offZ)                                                                                                         \
    {                                                                                                                                      \
        auto br = m_level->getBrightness(pos + BlockPos {offX, offY, offZ});                                                               \
        col = {static_cast<uint8_t>(br * 255.f), static_cast<uint8_t>(br * 255.f), static_cast<uint8_t>(br * 255.f), 255};                 \
    }

    TIME_MEASURE_BEGIN(GenMeshLoop)

    for (uint8_t x = 0; x < chunkSize; x++) {
        for (uint16_t y = 0; y < chunkHeight; y++) {
            for (uint8_t z = 0; z < chunkSize; z++) {
                auto globalPos = BlockPos {m_pos.x * chunkSize + x, y, m_pos.z * chunkSize + z};

                if (m_level->isSolidTile(globalPos)) {
                    auto pos = BlockPos {x, y, z};
                    Vector3 normal;
                    Color col = {255, 255, 255, 255};
                    BlockPos newPos;
                    auto uv = AssetManager::sharedState()->uvForBlockType(getBlock(pos));
                    // auto uv = UV {0, 0, 1, 1};

                    if (!isSolidTile({x, y + 1, z})) { // y+
                        CALC_COL(0, 1, 0)
                        normal = {0, 1, 0};
                        ADD_VERTEX(0, 1, 0, uv.startX, uv.startY)
                        ADD_VERTEX(0, 1, 1, uv.startX, uv.endY)
                        ADD_VERTEX(1, 1, 0, uv.endX, uv.startY)

                        ADD_VERTEX(0, 1, 1, uv.startX, uv.endY)
                        ADD_VERTEX(1, 1, 1, uv.endX, uv.endY)
                        ADD_VERTEX(1, 1, 0, uv.endX, uv.startY)
                    }

                    if (!isSolidTile({x, y - 1, z})) { // y-
                        CALC_COL(0, -1, 0)
                        normal = {0, -1, 0};
                        ADD_VERTEX(1, 0, 0, uv.endX, uv.startY)
                        ADD_VERTEX(0, 0, 1, uv.startX, uv.endY)
                        ADD_VERTEX(0, 0, 0, uv.startX, uv.startY)

                        ADD_VERTEX(1, 0, 0, uv.endX, uv.startY)
                        ADD_VERTEX(1, 0, 1, uv.endX, uv.endY)
                        ADD_VERTEX(0, 0, 1, uv.startX, uv.endY)
                    }

                    if (!m_level->isSolidTile(globalPos + BlockPos {0, 0, 1})) { // z+
                        CALC_COL(0, 0, 1)
                        normal = {0, 0, 1};
                        ADD_VERTEX(0, 0, 1, uv.startX, uv.endY)
                        ADD_VERTEX(1, 0, 1, uv.endX, uv.endY)
                        ADD_VERTEX(1, 1, 1, uv.endX, uv.startY)

                        ADD_VERTEX(0, 0, 1, uv.startX, uv.endY)
                        ADD_VERTEX(1, 1, 1, uv.endX, uv.startY)
                        ADD_VERTEX(0, 1, 1, uv.startX, uv.startY)
                    }

                    if (!m_level->isSolidTile(globalPos - BlockPos {0, 0, 1})) { // z-
                        CALC_COL(0, 0, -1)
                        normal = {0, 0, -1};
                        ADD_VERTEX(1, 1, 0, uv.startX, uv.startY)
                        ADD_VERTEX(1, 0, 0, uv.startX, uv.endY)
                        ADD_VERTEX(0, 0, 0, uv.endX, uv.endY)

                        ADD_VERTEX(0, 1, 0, uv.endX, uv.startY)
                        ADD_VERTEX(1, 1, 0, uv.startX, uv.startY)
                        ADD_VERTEX(0, 0, 0, uv.endX, uv.endY)
                    }

                    if (!m_level->isSolidTile(globalPos + BlockPos {1, 0, 0})) { // x+
                        CALC_COL(1, 0, 0)
                        normal = {1, 0, 0};
                        ADD_VERTEX(1, 0, 0, uv.endX, uv.endY)
                        ADD_VERTEX(1, 1, 0, uv.endX, uv.startY)
                        ADD_VERTEX(1, 0, 1, uv.startX, uv.endY)

                        ADD_VERTEX(1, 1, 0, uv.endX, uv.startY)
                        ADD_VERTEX(1, 1, 1, uv.startX, uv.startY)
                        ADD_VERTEX(1, 0, 1, uv.startX, uv.endY)
                    }

                    if (!m_level->isSolidTile(globalPos - BlockPos {1, 0, 0})) { // x-
                        CALC_COL(-1, 0, 0)
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

    TIME_MEASURE_END(GenMeshLoop)
    TIME_MEASURE_DBG(GenMeshLoop)

    TIME_MEASURE_BEGIN(GenMeshUpload)
    UploadMesh(&m_mesh, false);
    TIME_MEASURE_END(GenMeshUpload)
    TIME_MEASURE_DBG(GenMeshUpload)

    auto texture = AssetManager::sharedState()->textureForName("assets/terrain.png");

    TIME_MEASURE_BEGIN(GenMeshModel)
    m_model = LoadModelFromMesh(m_mesh);
    m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    TIME_MEASURE_END(GenMeshModel)
    TIME_MEASURE_DBG(GenMeshModel)

    m_built = true;

    TIME_MEASURE_END(GenMeshTotal)
    TIME_MEASURE_DBG(GenMeshTotal)
}

BlockTypes Chunk::getBlock(const BlockPos& pos) {
    if (!pos.isInChunk())
        return BlockTypes::Air;

    return m_data[pos.x][pos.y][pos.z];
}

void Chunk::setBlock(const BlockPos& pos, BlockTypes type) {
    if (!pos.isInChunk())
        return;

    m_data[pos.x][pos.y][pos.z] = type;
}

float Chunk::getBrightness(const BlockPos& pos) {
    if (!pos.isInChunk())
        return 1.f;

    return pos.y < m_lightDephts[pos.x][pos.z] ? .8f : 1.f;
}

void Chunk::calcLightDepths() {
    for (uint8_t x = 0; x < chunkSize; x++) {
        for (uint8_t z = 0; z < chunkSize; z++) {
            auto y = chunkHeight - 1;

            while (y > 0 && !isSolidTile({x, y, z})) {
                --y;
            }

            m_lightDephts[x][z] = y;
        }
    }
}

Mesh* Chunk::getMesh() {
    return &m_mesh;
}

Model* Chunk::getModel() {
    return &m_model;
}

void Chunk::cameraLook(Ray ray, HitResult& coll) {
    coll.coll = GetRayCollisionMesh(ray, m_model.meshes[0], MatrixTranslate(m_pos.x * chunkSize, 0.f, m_pos.z * chunkSize));
    if (coll.coll.hit) {
        auto normal = coll.coll.normal;
        if (normal == Vector3 {0, 1, 0})
            coll.face = Faces::Up;
        else if (normal == Vector3 {0, -1, 0})
            coll.face = Faces::Down;
        else if (normal == Vector3 {1, 0, 0})
            coll.face = Faces::Left;
        else if (normal == Vector3 {-1, 0, 0})
            coll.face = Faces::Right;
        else if (normal == Vector3 {0, 0, 1})
            coll.face = Faces::Back;
        else if (normal == Vector3 {0, 0, -1})
            coll.face = Faces::Front;

        auto pos = coll.coll.point;
        coll.blockPos = {static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z)};

        if (coll.face == Faces::Up)
            coll.blockPos.y--;
        if (coll.face == Faces::Back)
            coll.blockPos.z--;
        if (coll.face == Faces::Left)
            coll.blockPos.x--;
    }
}
