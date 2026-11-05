#pragma once
#include "Executor.hpp"
#include "model/Point.hpp"
#include <cstddef>

namespace adas {
class PoseHandler final {
public:
    explicit PoseHandler(const Pose& initialPose) noexcept;
    PoseHandler(const PoseHandler&) = delete;
    PoseHandler& operator=(const PoseHandler&) = delete;
    void Forward() noexcept;
    void Backward() noexcept;
    void RotateLeft() noexcept;
    void RotateRight() noexcept;
    bool IsReverse() const noexcept;
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
