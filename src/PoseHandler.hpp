#pragma once
#include "Executor.hpp"

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
    bool IsFast() const noexcept;
    Pose Query() const noexcept;
private:
    Pose pose;
    bool fast{false};
};
}  // namespace adas
