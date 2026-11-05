#include "executor/ExecutorImpl.hpp"
#include "command/CmderFactory.hpp"
#include <new>
#include <algorithm>

namespace adas {
Executor* Executor::NewExecutor(const Pose& initialPose) noexcept
{
    return new (std::nothrow) ExecutorImpl(initialPose);
}
Executor* Executor::NewExecutor(const Pose& initialPose, VehicleType type) noexcept
{
    return new (std::nothrow) ExecutorImpl(initialPose, type);
}
ExecutorImpl::ExecutorImpl(const Pose& initialPose, VehicleType type) noexcept
    : poseHandler(initialPose), vehicleType(type)
{
}
void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    const auto cmders = Singleton<CmderFactory>::Instance().GetCmders(commands, vehicleType);
    std::for_each(cmders.begin(), cmders.end(), [this](const Cmder& cmder) noexcept {
        cmder(poseHandler).DoOperate(poseHandler);
    });
}
Pose ExecutorImpl::Query() const noexcept
{
    return poseHandler.Query();
}
}  // namespace adas
