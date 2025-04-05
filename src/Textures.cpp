#include <Textures.hpp>
#include <iostream>
#include <glad/glad.h>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unordered_map<std::string, int> idMap; 

int Textures::loadTexture(const std::string& resourceName, int mode) {
    if (idMap.contains(resourceName)) {
        return idMap[resourceName];
    }

    // Generate a new texture id
    GLuint id;
    glGenTextures(1, &id);

    idMap.insert(std::make_pair(resourceName, id));
    std::cout << resourceName << " -> " << id << std::endl;

    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

    int width, height, channels;
    unsigned char* data = stbi_load(resourceName.c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "!!" << std::endl;
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return id;
}