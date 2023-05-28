#pragma once

#include <raylib.h>
#include <rlFPCamera.h>

#include <GL/glew.h>
#include <GL/GL.h>

#include <iostream>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <array>

namespace fs = std::filesystem;
using std::string, std::vector;

#include "logger.hpp"
#include "utils.hpp"
#include "AssetManager.hpp"