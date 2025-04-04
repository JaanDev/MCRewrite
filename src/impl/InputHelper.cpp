#include <impl/InputHelper.hpp>
#include <array>
#include "Game.hpp"

bool InputHelper::isMouseDown(int button) {
    return glfwGetMouseButton(Game::get().getWindow(), button);
}

bool InputHelper::isKeyDown(int key) {
    return glfwGetKey(Game::get().getWindow(), key);
}

bool InputHelper::isMousePressed(int button) {
    static std::array<bool, GLFW_MOUSE_BUTTON_LAST> lastMouseButtons = {false};

    bool isPressedNow = InputHelper::isMouseDown(button);
    bool wasPressedBefore = lastMouseButtons[button];
    
    if (isPressedNow) {
        if (wasPressedBefore)
            return false;

        lastMouseButtons[button] = true;

        return true;
    }

    lastMouseButtons[button] = false;

    return false;
}

bool InputHelper::isKeyPressed(int key) {
    static std::array<bool, GLFW_KEY_LAST> lastKeys = {false};

    bool isPressedNow = InputHelper::isKeyDown(key);
    bool wasPressedBefore = lastKeys[key];
    
    if (isPressedNow) {
        if (wasPressedBefore)
            return false;

        lastKeys[key] = true;

        return true;
    }

    lastKeys[key] = false;

    return false;
}
