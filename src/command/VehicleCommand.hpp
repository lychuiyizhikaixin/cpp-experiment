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
class SportsMoveCommand final {
public:
    ActionGroup operator()(const PoseHandler& pose) const
    {
        ActionGroup result;
        const int steps = pose.IsFast() ? 4 : 2;
        for (int i = 0; i < steps; ++i) { result.PushAction(Movement(pose)); }
        return result;
    }
};
class SportsTurnCommand final {
public:
    explicit SportsTurnCommand(bool left) noexcept : left(left) {}
    ActionGroup operator()(const PoseHandler& pose) const
    {
        ActionGroup result;
        if (pose.IsFast()) { result.PushAction(Movement(pose)); }
        if (left) {
            result.PushAction(pose.IsReverse() ? ActionType::ReverseTurnLeft : ActionType::TurnLeft);
        } else {
            result.PushAction(pose.IsReverse() ? ActionType::ReverseTurnRight : ActionType::TurnRight);
        }
        result.PushAction(Movement(pose));
        return result;
    }
private:
    bool left;
};
}  // namespace adas
