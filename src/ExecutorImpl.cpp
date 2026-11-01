#include "ExecutorImpl.hpp"
#include "Command.hpp"
#include <new>
#include <unordered_map>
#include <memory>

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
    std::unordered_map<char, std::unique_ptr<ICommand>> commanders;
    commanders.emplace('M', std::make_unique<MoveCommand>());
    commanders.emplace('L', std::make_unique<TurnLeftCommand>());
    commanders.emplace('R', std::make_unique<TurnRightCommand>());
    commanders.emplace('F', std::make_unique<FastCommand>());
    for (const char command : commands) {
        const auto it = commanders.find(command);
        if (it != commanders.end()) {
            it->second->DoOperate(poseHandler);
        }
    }
}
Pose ExecutorImpl::Query() const noexcept
{
    return poseHandler.Query();
}
}  // namespace adas
