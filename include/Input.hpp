#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace Input{
    extern std::unordered_map<int, bool> keypresses;
    extern glm::vec2 mousePos;
}