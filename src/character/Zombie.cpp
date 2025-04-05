#include <character/Zombie.hpp>
#include <impl/Math.hpp>
#include <glm/gtc/constants.hpp>
#include <Textures.hpp>
#include <glfw/glfw3.h>

Zombie::Zombie(Level& level, const glm::vec3& pos) : Entity(level), m_head({0, 0}), m_body({16, 16}), m_arm0({40, 16}), m_arm1({40, 16}), m_leg0({0, 16}), m_leg1({0, 16}) {
    m_rotA = (float)(Math::random() + 1.0) * 0.01f;
    m_pos = pos;
    m_timeOff = (float)Math::random() * 1239813.0f;
    m_rot = Math::random() * glm::pi<double>() * 2.0;
    m_speed = 1.f;
    // m_head = Cube({0, 0});
    m_head.addBox({-4.0f, -8.0f, -4.0f}, {8, 8, 8});
    
    // m_body = Cube({16, 16});
    m_body.addBox({-4.0f, 0.0f, -2.0f}, {8, 12, 4});
    
    // m_arm0 = Cube({40, 16});
    m_arm0.addBox({-3.0f, -2.0f, -2.0f}, {4, 12, 4});
    m_arm0.setPos({-5.0f, 2.0f, 0.0f});
    
    // m_arm1 = Cube({40, 16});
    m_arm1.addBox({-1.0f, -2.0f, -2.0f}, {4, 12, 4});
    m_arm1.setPos({5.0f, 2.0f, 0.0f});
    
    // m_leg0 = Cube({0, 16});
    m_leg0.addBox({-2.0f, 0.0f, -2.0f}, {4, 12, 4});
    m_leg0.setPos({-2.0f, 12.0f, 0.0f});
    
    // m_leg1 = Cube({0, 16});
    m_leg1.addBox({-2.0f, 0.0f, -2.0f}, {4, 12, 4});
    m_leg1.setPos({2.0f, 12.0f, 0.0f});
}

void Zombie::render(float a) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures::loadTexture("char.png", GL_NEAREST));
    double time = (glfwGetTime() * 10.0 * m_speed) + m_timeOff;
    float yy = ((-glm::abs(glm::sin(time * 0.6662))) * 5.0) - 23.0;
    m_head.render();
    // m_body.render();
    // m_arm0.render();
    // m_arm1.render();
    // m_leg0.render();
    // m_leg1.render();
    glDisable(GL_TEXTURE_2D);
}