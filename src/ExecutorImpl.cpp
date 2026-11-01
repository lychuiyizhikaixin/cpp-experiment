#include "ExecutorImpl.hpp"
#include "Command.hpp"
#include <new>

namespace adas {
Executor* Executor::NewExecutor(const Pose& initialPose) noexcept
{
    return new (std::nothrow) ExecutorImpl(initialPose);
}
ExecutorImpl::ExecutorImpl(const Pose& initialPose) noexcept : poseHandler(initialPose)
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
            commander->DoOperate(poseHandler);
        }
    }
}
Pose ExecutorImpl::Query() const noexcept
{
    return poseHandler.Query();
}
}  // namespace adas
