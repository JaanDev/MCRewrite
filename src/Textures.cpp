#include <Textures.hpp>
#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int lastId = INT_MIN;

int Textures::loadTexture(const std::string& resourceName, int mode) {
    // Generate a new texture id
    GLuint id;
    glGenTextures(1, &id);

    bind(id);

    // Set texture filter mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

    int width, height, channels;
    unsigned char* data = stbi_load(resourceName.c_str(), &width, &height, &channels, 4);
    
    if (!data) {
        std::cerr << "Texture load failed: " << resourceName << std::endl;
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return id;
}

void Textures::bind(int id) {
    if (id != lastId) {
        glBindTexture(GL_TEXTURE_2D, id);
        lastId = id;
    }
}