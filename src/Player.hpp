#pragma once
#include <Entity.hpp>

class Player : public Entity {
public:
    Player(Level& level);

    void tick() override;
};