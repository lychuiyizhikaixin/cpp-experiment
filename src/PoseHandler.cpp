#include "PoseHandler.hpp"
#include "Direction.hpp"

namespace adas {
PoseHandler::PoseHandler(const Pose& initialPose) noexcept
    : point(initialPose.x, initialPose.y), direction(FindDirection(initialPose.heading))
{
}
void PoseHandler::Move() noexcept
{
    point += DIRECTIONS[direction].step;
}
void PoseHandler::TurnLeft() noexcept
{
    direction = (direction + 3) % DIRECTIONS.size();
}
void PoseHandler::TurnRight() noexcept
{
    direction = (direction + 1) % DIRECTIONS.size();
}
void PoseHandler::Fast() noexcept
{
    fast = !fast;
}
bool PoseHandler::IsFast() const noexcept
{
    return fast;
}
Pose PoseHandler::Query() const noexcept
{
    return {point.GetX(), point.GetY(), DIRECTIONS[direction].heading};
}
}  // namespace adas
