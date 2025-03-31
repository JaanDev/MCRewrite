#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

class Game {
  public:
    Game();
  
    static Game& get() {
        static Game inst;
        return inst;
    }

    int start();

    inline auto getWindow() { return m_window; }

  private:
    GLFWwindow* m_window;
};