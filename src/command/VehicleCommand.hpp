#pragma once
#include "command/Command.hpp"

namespace adas {
class BusTurnCommand final {
public:
    explicit BusTurnCommand(bool left) noexcept : left(left) {}
    ActionGroup operator()(const PoseHandler& pose) const
    {
        auto result = MoveCommand{}(pose);
        if (left) {
            result.PushAction(pose.IsReverse() ? ActionType::ReverseTurnLeft : ActionType::TurnLeft);
        } else {
            result.PushAction(pose.IsReverse() ? ActionType::ReverseTurnRight : ActionType::TurnRight);
        }
        return result;
    }
private:
    bool left;
};
}  // namespace adas
