#include <impl/InputHelper.hpp>
#include <unordered_map>

// keyID, isPressed
std::unordered_map<int, bool> keyStates;
std::unordered_map<int, bool> mouseStates;

bool InputHelper::isKeyDown(int key) {
    return keyStates.find(key) != keyStates.end();
}

bool InputHelper::isKeyPressed(int key) {
    auto it = keyStates.find(key);

    if (it != keyStates.end() && !it->second) {
        it->second = true;
        return true;
    }

    return false;
}

bool InputHelper::isMouseDown(int button) {
    return mouseStates.find(button) != mouseStates.end();
}

bool InputHelper::isMousePressed(int button) {
    auto it = mouseStates.find(button);

    if (it != mouseStates.end() && !it->second) {
        it->second = true;
        return true;
    }

    return false;
}

void InputHelper::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UNKNOWN) {
        return;
    }
    
    auto it = keyStates.find(key);

    if (action == GLFW_PRESS && it == keyStates.end()) {
        keyStates.insert(std::make_pair(key, false));
    }
    
    if (action == GLFW_RELEASE && it != keyStates.end()) {
        keyStates.erase(key);
    }
}

void InputHelper::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto it = mouseStates.find(button);

    if (action == GLFW_PRESS && it == mouseStates.end()) {
        mouseStates.insert(std::make_pair(button, false));
    }
    
    if (action == GLFW_RELEASE && it != mouseStates.end()) {
        mouseStates.erase(button);
    }
}