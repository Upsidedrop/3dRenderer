#include "Input.hpp"

namespace Input{
    std::unordered_map<int, bool> keypresses = {};
    glm::vec2 mousePos = glm::vec2(0, 0);
    bool mouseButtons[3] = {false, false, false};
}