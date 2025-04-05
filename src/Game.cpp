#include "Game.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <ctime>
#include <Timer.hpp>
#include <Level.hpp>
#include <Player.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <DefaultShader.hpp>
#include <iostream>
#include <Textures.hpp>
#include <Chunk.hpp>
#include <Icon.hpp>
#include <impl/InputHelper.hpp>

Game::Game() {}

int Game::run() {
    static bool hasStarted = false;

    if (hasStarted) {
        return 0;
    }

    hasStarted = true;

    const int width = 1024;
    const int height = 768;

    if (!glfwInit()) {
        printf("GLFW init error!\n");
        return -1;
    }

    m_window = glfwCreateWindow(width, height, "Game", nullptr, nullptr);

    if (!m_window) {
        glfwTerminate();
        printf("Error while creating window\n");
        return -1;
    }

    GLFWimage icon = {16, 16, (unsigned char*)LWJGL_ICON_DATA_16x16};
    glfwSetWindowIcon(m_window, 1, &icon);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        printf("Error while loading GLAD(os)\n");
        return -1;
    }

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5f, 0.8f, 1.f, 0.f);
    glClearDepth(1.f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);

    m_defaultShader = createShaderProgram(vertexShader, fragmentShader);

    int texture = Textures::loadTexture("terrain.png", GL_NEAREST);

    if (!texture) {
        return -1;
    }

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Timer timer(60);
    Level level(256, 256, 64);
    Player player(level);

    int frames = 0;
    auto lastTime = std::chrono::steady_clock::now();
    glm::dvec2 prevMouse;
    glm::dvec2 mouse;
    HitResult hitResult;

    glfwGetCursorPos(m_window, &prevMouse.x, &prevMouse.y);

    glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.05f, 1000.0f);

    while (!glfwGetKey(m_window, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(m_window)) {
        timer.advanceTime();

        for (uint32_t i = 0; i < timer.getTicks(); ++i) {
            player.tick();

            for (auto& zombie : m_zombies) {
                zombie.tick();
            }    
        }

        glfwGetCursorPos(m_window, &mouse.x, &mouse.y);

        player.turn(glm::vec2(mouse.x - prevMouse.x, prevMouse.y - mouse.y));
        prevMouse = mouse;

        if (hitResult.hit && InputHelper::isMousePressed(GLFW_MOUSE_BUTTON_2)) {
            level.setTile(hitResult.pos, 0);
        }

        if (hitResult.hit && InputHelper::isMousePressed(GLFW_MOUSE_BUTTON_1)) {
            auto pos = hitResult.pos;

            // Get position of the tile using face direction
            if (hitResult.face == Faces::Down) {
                pos.y--;
            }
            if (hitResult.face == Faces::Up) {
                pos.y++;
            }
            if (hitResult.face == Faces::Back) {
                pos.z--;
            }
            if (hitResult.face == Faces::Front) {
                pos.z++;
            }
            if (hitResult.face == Faces::Left) {
                pos.x--;
            }
            if (hitResult.face == Faces::Right) {
                pos.x++;
            }

            // Set the tile
            level.setTile(pos, 1);
        }

        if (InputHelper::isKeyPressed(GLFW_KEY_ENTER)) {
            level.save();
        }

        if (InputHelper::isKeyPressed(GLFW_KEY_G)) {
            m_zombies.push_back(Zombie(level, player.getPos()));
        }

        // begin render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto& rot = glm::radians(player.getRot());
        const auto& pos = player.getPos();

        glm::mat4 view = glm::identity<glm::mat4>();
        view = glm::translate(view, {0, 0, -0.3f});
        view = glm::rotate(view, glm::radians(-player.getRot().y), {1, 0, 0});
        view = glm::rotate(view, glm::radians(player.getRot().x + 90), {0, 1, 0});
        view = glm::translate(view, -(player.getPosO() + (player.getPos() - player.getPosO()) * timer.getPartialTicks()));
        glm::mat4 mvp = projection * view;

        glm::vec3 direction = glm::vec3(cos(rot.x) * cos(rot.y), sin(rot.y), sin(rot.x) * cos(rot.y));

        glm::vec3 cameraPosition = glm::vec3(pos.x, pos.y, pos.z);

        glUseProgram(m_defaultShader);
        glUniformMatrix4fv(glGetUniformLocation(m_defaultShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(m_defaultShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(m_defaultShader, "cameraPos"), 1, glm::value_ptr(cameraPosition));
        glUniform1i(glGetUniformLocation(m_defaultShader, "useUColor"), 0);
        glUniform1i(glGetUniformLocation(m_defaultShader, "useTexture"), 1);
        glBindTexture(GL_TEXTURE_2D, texture);

        setModelMatrix(glm::mat4(1.f));

        level.render(mvp);

        for (auto& zombie : m_zombies) {
            zombie.render(timer.getPartialTicks());
        }

        hitResult = pick(cameraPosition, direction, level);

        if (hitResult.hit) {
            glUniform1i(glGetUniformLocation(m_defaultShader, "useUColor"), 1);
            glUniform1i(glGetUniformLocation(m_defaultShader, "useTexture"), 0);
            glUniform3f(glGetUniformLocation(m_defaultShader, "uColor"), 1.f, 1.f, 1.f);
            glUniform1f(glGetUniformLocation(m_defaultShader, "alpha"), (float)(std::sin(glfwGetTime() * 10) * 0.2f) + 0.4f);

            level.renderHit(hitResult);
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
        // end render

        frames++;

        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() >= 1000) {
            std::cout << frames << " fps, " << Chunk::updates << std::endl;

            Chunk::updates = 0;
            lastTime = currentTime;
            frames = 0;
        }
    }

    level.save();

    glfwDestroyWindow(m_window);
    glfwTerminate();

    return 0;
}

GLuint Game::createShader(const std::string_view& data, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char* ptr = data.data();
    glShaderSource(shader, 1, &ptr, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint Game::createShaderProgram(const std::string_view& vertexData, const std::string_view& fragmentData) {
    GLuint vertexShader = createShader(vertexData, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentData, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader link error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

HitResult Game::pick(const glm::vec3& start, const glm::vec3& direction, Level& level) {
    HitResult result;

    const glm::vec3 dir = glm::normalize(direction);
    glm::vec3 currentPos = start;

    // DDA (Digital Differential Analyzer)
    glm::ivec3 mapPos = glm::ivec3(floor(currentPos.x), floor(currentPos.y), floor(currentPos.z));
    glm::vec3 deltaDist = glm::vec3(std::abs(1.0f / dir.x), std::abs(1.0f / dir.y), std::abs(1.0f / dir.z));

    glm::ivec3 step;
    glm::vec3 sideDist;

    for (int i = 0; i < 3; ++i) {
        if (dir[i] < 0) {
            step[i] = -1;
            sideDist[i] = (currentPos[i] - mapPos[i]) * deltaDist[i];
        } else {
            step[i] = 1;
            sideDist[i] = (mapPos[i] + 1.0f - currentPos[i]) * deltaDist[i];
        }
    }

    // DDA cycle
    float traveled = 0.0f;
    while (traveled < 3.0f) {
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            traveled = sideDist.x;
            sideDist.x += deltaDist.x;
            mapPos.x += step.x;
            result.face = step.x < 0 ? Faces::Right : Faces::Left;
        } else if (sideDist.y < sideDist.z) {
            traveled = sideDist.y;
            sideDist.y += deltaDist.y;
            mapPos.y += step.y;
            result.face = step.y < 0 ? Faces::Up : Faces::Down;
        } else {
            traveled = sideDist.z;
            sideDist.z += deltaDist.z;
            mapPos.z += step.z;
            result.face = step.z < 0 ? Faces::Front : Faces::Back;
        }

        if (level.isSolidTile(mapPos)) {
            result.hit = true;
            result.pos = mapPos;
            break;
        }
    }

    return result;
}

void Game::setModelMatrix(const glm::mat4& model) {
    glUniformMatrix4fv(glGetUniformLocation(m_defaultShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
}