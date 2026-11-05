#pragma once
#include <cstdint>

namespace adas {
class Point final {
public:
    constexpr Point(std::int32_t x, std::int32_t y) noexcept : x(x), y(y) {}
    constexpr std::int32_t GetX() const noexcept { return x; }
    constexpr std::int32_t GetY() const noexcept { return y; }
    Point& operator+=(const Point& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
private:
    std::int32_t x;
    std::int32_t y;
};
}  // namespace adas
