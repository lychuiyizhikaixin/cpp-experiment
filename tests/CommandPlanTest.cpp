#include <gtest/gtest.h>
#include <memory>
#include "command/Command.hpp"
#include "PoseEq.hpp"

namespace adas {
TEST(CommandPlanTest, normal_move_is_one_forward_action)
{
    PoseHandler p({2, -3, 'E'});
    ASSERT_EQ((std::vector<ActionType>{ActionType::Forward}), MoveCommand{}(p).Actions());
}
TEST(CommandPlanTest, fast_move_is_two_separate_actions)
{
    PoseHandler p({2, -3, 'E'}); p.Fast();
    ASSERT_EQ((std::vector<ActionType>{ActionType::Forward, ActionType::Forward}), MoveCommand{}(p).Actions());
}
TEST(CommandPlanTest, fast_reverse_left_moves_before_turning)
{
    PoseHandler p({2, -3, 'E'}); p.Fast(); p.Reverse();
    ASSERT_EQ((std::vector<ActionType>{ActionType::Backward, ActionType::ReverseTurnLeft}),
              TurnLeftCommand{}(p).Actions());
}
TEST(CommandPlanTest, planning_does_not_change_the_receiver)
{
    PoseHandler p({2, -3, 'E'}); p.Fast();
    const auto group = TurnRightCommand{}(p);
    ASSERT_EQ((Pose{2, -3, 'E'}), p.Query());
    group.DoOperate(p);
    ASSERT_EQ((Pose{3, -3, 'S'}), p.Query());
}
TEST(CommandPlanTest, fast_and_reverse_are_executed_as_actions)
{
    PoseHandler p({0, 0, 'N'});
    const auto fast = FastCommand{}(p); const auto reverse = ReverseCommand{}(p);
    ASSERT_FALSE(p.IsFast()); ASSERT_FALSE(p.IsReverse());
    fast.DoOperate(p); reverse.DoOperate(p);
    ASSERT_TRUE(p.IsFast()); ASSERT_TRUE(p.IsReverse());
}
TEST(CommandPlanTest, state_is_read_between_commands_not_once_for_the_whole_input)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor());
    p->Execute("FMFRM");
    ASSERT_EQ((Pose{1, 2, 'E'}), p->Query());
}
}  // namespace adas
