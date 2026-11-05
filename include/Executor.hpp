#pragma once
#include <cstdint>
#include <string>

namespace adas {
enum class VehicleType { Normal, Bus, SportsCar };

struct Pose {
    std::int32_t x;
    std::int32_t y;
    char heading;
};
class Executor {
public:
    // Caller owns the returned pointer. A null pointer indicates allocation failure.
    static Executor* NewExecutor(const Pose& pose = {0, 0, 'N'}) noexcept;
    static Executor* NewExecutor(const Pose& pose, VehicleType type) noexcept;
    Executor() = default;
    virtual ~Executor() noexcept = default;
    Executor(const Executor&) = delete;
    Executor& operator=(const Executor&) = delete;
    virtual void Execute(const std::string& commands) noexcept = 0;
    virtual Pose Query() const noexcept = 0;
};
}  // namespace adas
