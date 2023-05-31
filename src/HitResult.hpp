#pragma once
#include "includes.hpp"

struct HitResult {
    RayCollision coll;
    Faces face;
    BlockPos blockPos;
};