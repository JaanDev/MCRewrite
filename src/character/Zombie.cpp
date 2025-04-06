#include <character/Zombie.hpp>
#include <impl/Math.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Textures.hpp>
#include <glfw/glfw3.h>
#include <Game.hpp>

Zombie::Zombie(Level& level, const glm::vec3& pos) : Entity(level), m_head({0, 0}), m_body({16, 16}), m_arm0({40, 16}), m_arm1({40, 16}), m_leg0({0, 16}), m_leg1({0, 16}) {
    m_rotA = (float)(Math::random() + 1.0) * 0.01f;
    m_pos = pos;
    m_timeOff = (float)Math::random() * 1239813.0f;
    m_rot = Math::random() * glm::pi<double>() * 2.0;
    m_speed = 1.f;

    // Vertex* vertices = new Vertex[24 * 6];
    
    // m_head.init({0, 0}, {-4.0f, -8.0f, -4.0f}, {8, 8, 8}, vertices);
    // vertices += 24;

    m_head.setBox({-4.f, -8.f, -4.f}, {8, 8, 8});
    m_body.setBox({-4.0f, 0.0f, -2.0f}, {8, 12, 4});

    m_arm0.setBox({-3.0f, -2.0f, -2.0f}, {4, 12, 4});
    m_arm0.setPos({-5.0f, 2.0f, 0.0f});
    
    m_arm1.setBox({-1.0f, -2.0f, -2.0f}, {4, 12, 4});
    m_arm1.setPos({5.0f, 2.0f, 0.0f});

    m_leg0.setBox({-2.0f, 0.0f, -2.0f}, {4, 12, 4});
    m_leg0.setPos({-2.0f, 12.0f, 0.0f});

    m_leg1.setBox({-2.0f, 0.0f, -2.0f}, {4, 12, 4});
    m_leg1.setPos({2.0f, 12.0f, 0.0f});
}

void Zombie::tick() { 
    Entity::tick();

    m_rot += m_rotA;
    m_rotA *= 0.99;
    // wow...
    m_rotA = m_rotA + ((Math::random() - Math::random()) * Math::random() * Math::random() * 0.009999999776482582);
    
    float xa = glm::sin(m_rot);
    float ya = glm::cos(m_rot);

    if (m_onGround && Math::random() < 0.01) {
        m_motion.y = 0.12f;
    }

    moveRelative(-xa, -ya, m_onGround ? 0.02f : 0.005f);

    m_motion.y -= 0.005;
    move(m_motion);

    m_motion *= glm::vec3(0.91f, 0.98f, 0.91f);
    
    if (m_pos.y > 100.0f) {
        resetPos();
    }
    
    if (m_onGround) {
        m_motion.x *= 0.8f;
        m_motion.z *= 0.8f;
    }
}

void Zombie::render() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures::loadTexture("char.png", GL_NEAREST));
    auto& game = Game::get();
    glm::mat4 model = game.getModelMatrix();
    auto bac = model;
    double time = (glfwGetTime() * 10.0 * m_speed) + m_timeOff;
    float yy = ((-glm::abs(glm::sin(time * 0.6662))) * 5.0) - 23.0;
    float a = game.getTimer().getPartialTicks();
    float scale = 7.f / 120.f;
    model = glm::translate(model, m_prevPos + ((m_pos - m_prevPos) * a));
    model = glm::scale(model, glm::vec3(scale, -scale, scale));
    model = glm::translate(model, glm::vec3(0, yy, 0));
    model = glm::rotate(model, m_rot, glm::vec3(0.0f, 1.0f, 0.0f));

    game.setModelMatrix(model);

    m_head.setRot({glm::sin(time) * 0.8f, glm::sin(time * 0.83), 0.f});
    m_arm0.setRot({glm::sin(time * 0.6662 + glm::pi<double>()) * 2.0f, 0.f, glm::sin(time * 0.2312) + 1.0});
    m_arm1.setRot({glm::sin(time * 0.6662) * 2.0f, 0.f, glm::sin(time * 0.2812) - 1.0});
    m_leg0.setRot({glm::sin(time * 0.6662) * 1.4f, 0.f, 0.f});
    m_leg1.setRot({glm::sin(time * 0.6662 + glm::pi<double>()) * 1.4f, 0.f, 0.f});

    m_head.render();
    m_body.render();
    m_arm0.render();
    m_arm1.render();
    m_leg0.render();
    m_leg1.render();
    game.setModelMatrix(bac);
    glDisable(GL_TEXTURE_2D);
}