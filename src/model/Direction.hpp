#pragma once
#include "model/Point.hpp"
#include <array>
#include <cstddef>

namespace adas {
struct Direction final {
    char heading;
    Point step;
};
// Clockwise order: adding 1 turns right, adding 3 turns left.
inline constexpr std::array<Direction, 4> DIRECTIONS{{
    {'E', {1, 0}}, {'S', {0, -1}}, {'W', {-1, 0}}, {'N', {0, 1}},
}};
inline std::size_t FindDirection(char heading) noexcept
{
    for (std::size_t i = 0; i < DIRECTIONS.size(); ++i) {
        if (DIRECTIONS[i].heading == heading) {
            return i;
        }
    }
    return 0;  // The course contract supplies a valid E/S/W/N heading.
}
}  // namespace adas
