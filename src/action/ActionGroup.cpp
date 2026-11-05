#include "action/ActionGroup.hpp"
#include <array>
#include <cstddef>

namespace adas {
namespace {
struct ForwardAction { void operator()(PoseHandler& p) const noexcept { p.Forward(); } };
struct BackwardAction { void operator()(PoseHandler& p) const noexcept { p.Backward(); } };
struct TurnLeftAction { void operator()(PoseHandler& p) const noexcept { p.RotateLeft(); } };
struct ReverseTurnLeftAction { void operator()(PoseHandler& p) const noexcept { p.RotateRight(); } };
struct TurnRightAction { void operator()(PoseHandler& p) const noexcept { p.RotateRight(); } };
struct ReverseTurnRightAction { void operator()(PoseHandler& p) const noexcept { p.RotateLeft(); } };
struct FastAction { void operator()(PoseHandler& p) const noexcept { p.Fast(); } };
struct ReverseAction { void operator()(PoseHandler& p) const noexcept { p.Reverse(); } };
}
void ActionGroup::DoOperate(PoseHandler& poseHandler) const noexcept
{
    using Action = void (*)(PoseHandler&) noexcept;
    static constexpr std::array<Action, 8> operations{{
        [](PoseHandler& p) noexcept { ForwardAction{}(p); },
        [](PoseHandler& p) noexcept { BackwardAction{}(p); },
        [](PoseHandler& p) noexcept { TurnLeftAction{}(p); },
        [](PoseHandler& p) noexcept { ReverseTurnLeftAction{}(p); },
        [](PoseHandler& p) noexcept { TurnRightAction{}(p); },
        [](PoseHandler& p) noexcept { ReverseTurnRightAction{}(p); },
        [](PoseHandler& p) noexcept { FastAction{}(p); },
        [](PoseHandler& p) noexcept { ReverseAction{}(p); },
    }};
    for (const auto action : actions) {
        operations[static_cast<std::size_t>(action)](poseHandler);
    }
}
}  // namespace adas
