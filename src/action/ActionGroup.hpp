#pragma once
#include "model/PoseHandler.hpp"
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace adas {
enum class ActionType : std::uint16_t {
    Forward, Backward, TurnLeft, ReverseTurnLeft, TurnRight, ReverseTurnRight, Fast, Reverse
};
class ActionGroup final {
public:
    ActionGroup() = default;
    ActionGroup(std::initializer_list<ActionType> actions) : actions(actions) {}
    void PushAction(ActionType action) { actions.push_back(action); }
    void DoOperate(PoseHandler& poseHandler) const noexcept;
    const std::vector<ActionType>& Actions() const noexcept { return actions; }
private:
    std::vector<ActionType> actions;
};
}  // namespace adas
