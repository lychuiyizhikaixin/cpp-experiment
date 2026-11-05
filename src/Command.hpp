#pragma once
#include "ActionGroup.hpp"

namespace adas {
inline ActionType Movement(const PoseHandler& pose) noexcept
{
    return pose.IsReverse() ? ActionType::Backward : ActionType::Forward;
}
class MoveCommand final {
public:
    ActionGroup operator()(const PoseHandler& pose) const
    {
        ActionGroup result;
        if (pose.IsFast()) { result.PushAction(Movement(pose)); }
        result.PushAction(Movement(pose));
        return result;
    }
};
class TurnLeftCommand final {
public:
    ActionGroup operator()(const PoseHandler& pose) const
    {
        ActionGroup result;
        if (pose.IsFast()) { result.PushAction(Movement(pose)); }
        result.PushAction(pose.IsReverse() ? ActionType::ReverseTurnLeft : ActionType::TurnLeft);
        return result;
    }
};
class TurnRightCommand final {
public:
    ActionGroup operator()(const PoseHandler& pose) const
    {
        ActionGroup result;
        if (pose.IsFast()) { result.PushAction(Movement(pose)); }
        result.PushAction(pose.IsReverse() ? ActionType::ReverseTurnRight : ActionType::TurnRight);
        return result;
    }
};
class TurnRoundCommand final {
public:
    ActionGroup operator()(const PoseHandler& pose) const
    {
        if (pose.IsReverse()) { return {}; }
        ActionGroup result;
        if (pose.IsFast()) { result.PushAction(ActionType::Forward); }
        result.PushAction(ActionType::TurnLeft);
        result.PushAction(ActionType::Forward);
        result.PushAction(ActionType::TurnLeft);
        return result;
    }
};
class FastCommand final {
public:
    ActionGroup operator()(const PoseHandler&) const { return {ActionType::Fast}; }
};
class ReverseCommand final {
public:
    ActionGroup operator()(const PoseHandler&) const { return {ActionType::Reverse}; }
};
}  // namespace adas
