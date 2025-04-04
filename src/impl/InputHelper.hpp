#pragma once
#define MAX_KEY_QUEUE 16
#include <GLFW/glfw3.h>

namespace InputHelper {
    bool isKeyDown(int key);
    bool isKeyPressed(int key);
    bool isMouseDown(int button);
    bool isMousePressed(int button);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
}