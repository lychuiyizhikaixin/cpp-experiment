#include "PoseHandler.hpp"
#include "Direction.hpp"

namespace adas {
PoseHandler::PoseHandler(const Pose& initialPose) noexcept
    : point(initialPose.x, initialPose.y), direction(FindDirection(initialPose.heading))
{
}
void PoseHandler::Move() noexcept
{
    const auto& step = DIRECTIONS[direction].step;
    const int sign = reverse ? -1 : 1;
    point += Point{step.GetX() * sign, step.GetY() * sign};
}
void PoseHandler::TurnLeft() noexcept
{
    const std::size_t offset = reverse ? 1 : 3;
    direction = (direction + offset) % DIRECTIONS.size();
}
void PoseHandler::TurnRight() noexcept
{
    const std::size_t offset = reverse ? 3 : 1;
    direction = (direction + offset) % DIRECTIONS.size();
}
void PoseHandler::Fast() noexcept
{
    fast = !fast;
}
bool PoseHandler::IsFast() const noexcept
{
    return fast;
}
void PoseHandler::Reverse() noexcept
{
    reverse = !reverse;
}
Pose PoseHandler::Query() const noexcept
{
    return {point.GetX(), point.GetY(), DIRECTIONS[direction].heading};
}
}  // namespace adas
