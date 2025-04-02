#include "Level.hpp"
#include <Frustum.hpp>
#include <Chunk.hpp>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <GLFW/glfw3.h>
#include <Tile.hpp>
#include <fstream>
#include <miniz.h>
#include <iostream>

Level::Level(int width, int height, int depth) : m_width(width), m_height(height), m_depth(depth), m_blocks(width * height * depth), m_lightDepths(width * height) {
    if (!load()) {
        // Fill level with tiles
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < depth; y++) {
                for (int z = 0; z < height; z++) {
                    // Calculate index from x, y and z
                    int index = (y * m_height + z) * m_width + x;

                    // Fill level with tiles
                    m_blocks[index] = (uint8_t)((y <= depth * 2 / 3) ? 1 : 0);
                }
            }
        }
    }

    glm::ivec3 chunkCount = {m_width, m_depth, m_height};
    chunkCount /= CHUNK_SIZE;

    m_chunks.resize(chunkCount.x * chunkCount.y * chunkCount.z);

    for (int x = 0; x < chunkCount.x; x++) {
        for (int y = 0; y < chunkCount.y; y++) {
            for (int z = 0; z < chunkCount.z; z++) {
                glm::ivec3 min = {
                    x * CHUNK_SIZE,
                    y * CHUNK_SIZE,
                    z * CHUNK_SIZE
                };

                glm::ivec3 max = {
                    std::min(m_width, (x + 1) * CHUNK_SIZE),
                    std::min(m_depth, (y + 1) * CHUNK_SIZE),
                    std::min(m_height, (z + 1) * CHUNK_SIZE)
                };

                m_chunks[(x + y * chunkCount.x) * chunkCount.z + z] = std::make_shared<Chunk>(*this, min, max);
            }
        }
    }

    calcLightDepths(0, 0, width, height);
}

bool Level::isSolidTile(glm::ivec3 pos) {
    if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= m_width || pos.y >= m_depth || pos.z >= m_height) {
        return false;
    }

    int index = (pos.y * m_height + pos.z) * m_width + pos.x;

    return m_blocks[index] != 0;
}

void Level::render(const glm::mat4& VP) {
    auto frustum = Frustum::get();
    frustum->calculateFrustum(VP);

    Chunk::buildThisFrame = 0;

    for (const auto& chunk : m_chunks) {
        if (frustum->cubeInFrustum(chunk->getBounds())) {
            chunk->render();
        }
    }
}

void Level::renderHit(const HitResult& hit) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_CURRENT_BIT);

    glBegin(GL_QUADS);

    switch (hit.face) {
        case Faces::Up:
            glVertex3f(hit.pos.x,     hit.pos.y + 1, hit.pos.z + 1);
            glVertex3f(hit.pos.x + 1, hit.pos.y + 1, hit.pos.z + 1);
            glVertex3f(hit.pos.x + 1, hit.pos.y + 1, hit.pos.z    );
            glVertex3f(hit.pos.x,     hit.pos.y + 1, hit.pos.z    );
            break;

        case Faces::Down:
            glVertex3f(hit.pos.x,     hit.pos.y, hit.pos.z    );
            glVertex3f(hit.pos.x + 1, hit.pos.y, hit.pos.z    );
            glVertex3f(hit.pos.x + 1, hit.pos.y, hit.pos.z + 1);
            glVertex3f(hit.pos.x,     hit.pos.y, hit.pos.z + 1);
            break;

        case Faces::Right:
            glVertex3f(hit.pos.x + 1, hit.pos.y,     hit.pos.z    );
            glVertex3f(hit.pos.x + 1, hit.pos.y + 1, hit.pos.z    );
            glVertex3f(hit.pos.x + 1, hit.pos.y + 1, hit.pos.z + 1);
            glVertex3f(hit.pos.x + 1, hit.pos.y,     hit.pos.z + 1);
            break;

        case Faces::Left:
            glVertex3f(hit.pos.x, hit.pos.y,     hit.pos.z    );
            glVertex3f(hit.pos.x, hit.pos.y    , hit.pos.z + 1);
            glVertex3f(hit.pos.x, hit.pos.y + 1, hit.pos.z + 1);
            glVertex3f(hit.pos.x, hit.pos.y + 1, hit.pos.z    );
            break;

        case Faces::Front:
            glVertex3f(hit.pos.x,     hit.pos.y,     hit.pos.z + 1);
            glVertex3f(hit.pos.x + 1, hit.pos.y,     hit.pos.z + 1);
            glVertex3f(hit.pos.x + 1, hit.pos.y + 1, hit.pos.z + 1);
            glVertex3f(hit.pos.x,     hit.pos.y + 1, hit.pos.z + 1);
            break;

        case Faces::Back:
            glVertex3f(hit.pos.x,      hit.pos.y,    hit.pos.z);
            glVertex3f(hit.pos.x,      hit.pos.y+ 1, hit.pos.z);
            glVertex3f(hit.pos.x + 1, hit.pos.y + 1, hit.pos.z);
            glVertex3f(hit.pos.x + 1, hit.pos.y,     hit.pos.z);
            break;
    }

    glEnd();

    glDisable(GL_BLEND);
}

void Level::calcLightDepths(int minX, int minZ, int maxX, int maxZ) {
    for (int x = minX; x < minX + maxX; x++) {
        for (int z = minZ; z < minZ + maxZ; z++) {
            int prevDepth = m_lightDepths[x + z * m_width];

            int depth = m_depth - 1;
            while (depth > 0 && !isSolidTile(glm::vec3(x, depth, z))) {
                depth--;
            }

            m_lightDepths[x + z * m_width] = depth;

            if (prevDepth != depth) {
                int minTileChangeY = std::min(prevDepth, depth);
                int maxTileChangeY = std::max(prevDepth, depth);

                rebuildChunks(glm::ivec3(x - 1, minTileChangeY - 1, z - 1), glm::ivec3(x + 1, maxTileChangeY + 1, z + 1));
            }
        }
    }
}

void Level::rebuildChunks(glm::ivec3 min, glm::ivec3 max) {
    min /= CHUNK_SIZE;
    max /= CHUNK_SIZE;

    min.x = std::max(min.x, 0);
    min.y = std::max(min.y, 0);
    min.z = std::max(min.z, 0);

    max.x = std::min(max.x, m_width / CHUNK_SIZE - 1);
    max.y = std::min(max.y, m_depth / CHUNK_SIZE - 1);
    max.z = std::min(max.z, m_height / CHUNK_SIZE - 1);

    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            for (int z = min.z; z <= max.z; z++) {
                auto chunk = m_chunks[(x + y * (m_width / CHUNK_SIZE)) * (m_height / CHUNK_SIZE) + z];
                chunk->rebuild();
            }
        }
    }
}

void Level::setTile(glm::ivec3 pos, int id) {
    if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= m_width || pos.y >= m_depth || pos.z >= m_height) {
        return;
    }

    m_blocks[(pos.y * m_height + pos.z) * m_width + pos.x] = (uint8_t)id;

    calcLightDepths(pos.x, pos.z, 1, 1);
    rebuildChunks(pos - 1, pos + 1);
}

float Level::getBrightness(const glm::ivec3& pos) {
    float dark = 0.8f;
    float light = 1.0f;

    if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= m_width || pos.y >= m_depth || pos.z >= m_height) {
        return light;
    }

    if (pos.y < m_lightDepths[pos.x + pos.z * m_width]) {
        return dark;
    }

    return light;
}

std::vector<AABB> Level::getCubes(const AABB& other) {
    std::vector<AABB> aabbs;

    auto pos0 = other.min();
    auto pos1 = other.max() + 1.f;

    if (pos0.x < 0) {
        pos0.x = 0;
    }

    if (pos0.y < 0) {
        pos0.y = 0;
    }

    if (pos0.z < 0) {
        pos0.z = 0;
    }

    if (pos1.x > m_width) {
        pos1.x = m_width;
    }

    if (pos1.y > m_depth) {
        pos1.y = m_depth;
    }
    
    if (pos1.z > m_height) {
        pos1.z = m_height;
    }

    for (int x = pos0.x; x < pos1.x; x++) {
        for (int y = pos0.y; y < pos1.y; y++) {
            for (int z = pos0.z; z < pos1.z; z++) {
                if (isSolidTile({x, y, z})) {
                    aabbs.push_back(AABB(glm::vec3(x, y, z), glm::vec3(x, y, z) + 1.f));
                }
            }
        }
    }

    return aabbs;
}

void Level::save() {
    auto file = std::ofstream("level.dat", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file level.dat" << std::endl;
        return;
    }

    mz_stream stream;
    memset(&stream, 0, sizeof(stream));
    stream.next_in = m_blocks.data();
    stream.avail_in = m_blocks.size();

    int status = mz_deflateInit2(
        &stream,
        MZ_DEFAULT_COMPRESSION,
        MZ_DEFLATED,
        -MZ_DEFAULT_WINDOW_BITS,
        9,
        MZ_DEFAULT_STRATEGY
    );

    if (status != MZ_OK) {
        std::cerr << "GZIP init failed: " << zError(status) << std::endl;
        file.close();
        return;
    }

    file.write("\x1f\x8b\x08\0\0\0\0\0\0\xff", 10);

    uint8_t out_buffer[4096];
    do {
        stream.next_out = out_buffer;
        stream.avail_out = sizeof(out_buffer);

        status = mz_deflate(&stream, MZ_FINISH);
        if (status != MZ_OK && status != MZ_STREAM_END) {
            std::cerr << "Compression error: " << status << std::endl;
            mz_deflateEnd(&stream);
            file.close();
            return;
        }

        file.write((const char*)out_buffer, sizeof(out_buffer) - stream.avail_out);
    } while (status != MZ_STREAM_END);

    mz_deflateEnd(&stream);
    file.close();
}

bool Level::load() {
    auto file = std::basic_ifstream<uint8_t>("level.dat", std::ios::binary);
    if (!file) {
        return false;
    }

    mz_stream stream = {0};
    if (mz_inflateInit2(&stream, -MZ_DEFAULT_WINDOW_BITS) != MZ_OK) {
        file.close();
        return false;
    }

    std::vector<uint8_t> buf((std::istreambuf_iterator<uint8_t>(file)), std::istreambuf_iterator<uint8_t>());

    stream.next_out = m_blocks.data();
    stream.avail_out = m_blocks.size();
    stream.next_in = buf.data() + 10;
    stream.avail_in = buf.size() - 10;

    int status = mz_inflate(&stream, MZ_NO_FLUSH);
    if (status != MZ_OK && status != MZ_STREAM_END) {
        return false;
    }

    mz_inflateEnd(&stream);
    file.close();
    return true;
}