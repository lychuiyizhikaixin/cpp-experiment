#pragma once
#include "Executor.hpp"
#include "model/PoseHandler.hpp"

namespace adas {
class ExecutorImpl final : public Executor {
public:
    explicit ExecutorImpl(const Pose& initialPose, VehicleType type = VehicleType::Normal) noexcept;
    ~ExecutorImpl() noexcept override = default;
    ExecutorImpl(const ExecutorImpl&) = delete;
    ExecutorImpl& operator=(const ExecutorImpl&) = delete;
    void Execute(const std::string& commands) noexcept override;
    Pose Query() const noexcept override;
private:
    PoseHandler poseHandler;
    VehicleType vehicleType;
};
}  // namespace adas
