#include "ExecutorImpl.hpp"
#include "Command.hpp"
#include <new>
#include <unordered_map>
#include <functional>

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
    static const std::unordered_map<char, std::function<void(PoseHandler&)>> commanders{
        {'M', MoveCommand{}},
        {'L', TurnLeftCommand{}},
        {'R', TurnRightCommand{}},
        {'F', FastCommand{}},
        {'B', ReverseCommand{}},
    };
    for (const char command : commands) {
        const auto it = commanders.find(command);
        if (it != commanders.end()) {
            it->second(poseHandler);
        }
    }
}
Pose ExecutorImpl::Query() const noexcept
{
    return poseHandler.Query();
}
}  // namespace adas
