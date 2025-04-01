#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string_view>
#include <utils.hpp>
#include <glm/glm.hpp>
#include <Level.hpp>

class Game {
public:
    Game();
  
    static Game& get() {
        static Game inst;
        return inst;
    }

    int run();

    inline auto getWindow() const { return m_window; }

    GLuint createShader(const std::string_view& data, GLenum shaderType);
    GLuint createShaderProgram(const std::string_view& vertexData, const std::string_view& fragmentData);

    HitResult pick(const glm::vec3& start, const glm::vec3& direction, Level& level);

private:
    GLFWwindow* m_window;
    GLuint m_defaultShader;
};