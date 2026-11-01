#pragma once
#include "Executor.hpp"
#include "Point.hpp"
#include <cstddef>

namespace adas {
class PoseHandler final {
public:
    explicit PoseHandler(const Pose& initialPose) noexcept;
    PoseHandler(const PoseHandler&) = delete;
    PoseHandler& operator=(const PoseHandler&) = delete;
    void Move() noexcept;
    void TurnLeft() noexcept;
    void TurnRight() noexcept;
    void Fast() noexcept;
    void Reverse() noexcept;
    bool IsFast() const noexcept;
    Pose Query() const noexcept;
private:
    Point point;
    std::size_t direction;
    bool fast{false};
    bool reverse{false};
};
}  // namespace adas
