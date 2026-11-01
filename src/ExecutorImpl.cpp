#include "ExecutorImpl.hpp"
#include "Command.hpp"
#include <new>

namespace adas {
Executor* Executor::NewExecutor(const Pose& initialPose) noexcept
{
    return new (std::nothrow) ExecutorImpl(initialPose);
}
ExecutorImpl::ExecutorImpl(const Pose& initialPose) noexcept : pose(initialPose)
{
}
void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    const MoveCommand moveCommand;
    const TurnLeftCommand turnLeftCommand;
    const TurnRightCommand turnRightCommand;
    const FastCommand fastCommand;
    for (const char command : commands) {
        const ICommand* commander = nullptr;
        if (command == 'M') {
            commander = &moveCommand;
        }
        else if (command == 'L') {
            commander = &turnLeftCommand;
        }
        else if (command == 'R') {
            commander = &turnRightCommand;
        }
        else if (command == 'F') {
            commander = &fastCommand;
        }
        if (commander != nullptr) {
            commander->DoOperate(*this);
        }
    }
}
void ExecutorImpl::Move() noexcept
{
    if (pose.heading == 'E') {
        ++pose.x;
    }
    else if (pose.heading == 'W') {
        --pose.x;
    }
    else if (pose.heading == 'N') {
        ++pose.y;
    }
    else if (pose.heading == 'S') {
        --pose.y;
    }
}
void ExecutorImpl::TurnLeft() noexcept
{
    if (pose.heading == 'E') {
        pose.heading = 'N';
    }
    else if (pose.heading == 'N') {
        pose.heading = 'W';
    }
    else if (pose.heading == 'W') {
        pose.heading = 'S';
    }
    else if (pose.heading == 'S') {
        pose.heading = 'E';
    }
}
void ExecutorImpl::TurnRight() noexcept
{
    if (pose.heading == 'E') {
        pose.heading = 'S';
    }
    else if (pose.heading == 'S') {
        pose.heading = 'W';
    }
    else if (pose.heading == 'W') {
        pose.heading = 'N';
    }
    else if (pose.heading == 'N') {
        pose.heading = 'E';
    }
}
void ExecutorImpl::Fast() noexcept
{
    fast = !fast;
}
bool ExecutorImpl::IsFast() const noexcept
{
    return fast;
}
Pose ExecutorImpl::Query() const noexcept
{
    return pose;
}
}  // namespace adas
