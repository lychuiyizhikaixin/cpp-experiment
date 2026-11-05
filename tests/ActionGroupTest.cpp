#include <gtest/gtest.h>
#include "../src/ActionGroup.hpp"
#include "PoseEq.hpp"

namespace adas {
TEST(ActionGroupTest, empty_group_keeps_pose)
{
    PoseHandler p({2, -3, 'E'}); ActionGroup{}.DoOperate(p);
    ASSERT_EQ((Pose{2, -3, 'E'}), p.Query());
}
TEST(ActionGroupTest, forward_moves_one_grid)
{
    PoseHandler p({2, -3, 'E'}); ActionGroup{ActionType::Forward}.DoOperate(p);
    ASSERT_EQ((Pose{3, -3, 'E'}), p.Query());
}
TEST(ActionGroupTest, backward_moves_one_grid)
{
    PoseHandler p({2, -3, 'N'}); ActionGroup{ActionType::Backward}.DoOperate(p);
    ASSERT_EQ((Pose{2, -4, 'N'}), p.Query());
}
TEST(ActionGroupTest, left_changes_heading_only)
{
    PoseHandler p({2, -3, 'E'}); ActionGroup{ActionType::TurnLeft}.DoOperate(p);
    ASSERT_EQ((Pose{2, -3, 'N'}), p.Query());
}
TEST(ActionGroupTest, right_changes_heading_only)
{
    PoseHandler p({2, -3, 'E'}); ActionGroup{ActionType::TurnRight}.DoOperate(p);
    ASSERT_EQ((Pose{2, -3, 'S'}), p.Query());
}
TEST(ActionGroupTest, reverse_left_is_a_right_rotation)
{
    PoseHandler p({2, -3, 'E'}); ActionGroup{ActionType::ReverseTurnLeft}.DoOperate(p);
    ASSERT_EQ((Pose{2, -3, 'S'}), p.Query());
}
TEST(ActionGroupTest, reverse_right_is_a_left_rotation)
{
    PoseHandler p({2, -3, 'E'}); ActionGroup{ActionType::ReverseTurnRight}.DoOperate(p);
    ASSERT_EQ((Pose{2, -3, 'N'}), p.Query());
}
TEST(ActionGroupTest, fast_toggles_without_moving)
{
    PoseHandler p({2, -3, 'E'}); ActionGroup{ActionType::Fast}.DoOperate(p);
    ASSERT_TRUE(p.IsFast());
    ASSERT_EQ((Pose{2, -3, 'E'}), p.Query());
    ActionGroup{ActionType::Fast}.DoOperate(p); ASSERT_FALSE(p.IsFast());
}
TEST(ActionGroupTest, reverse_toggles_without_moving)
{
    PoseHandler p({2, -3, 'E'}); ActionGroup{ActionType::Reverse}.DoOperate(p);
    ASSERT_TRUE(p.IsReverse());
    ASSERT_EQ((Pose{2, -3, 'E'}), p.Query());
    ActionGroup{ActionType::Reverse}.DoOperate(p); ASSERT_FALSE(p.IsReverse());
}
TEST(ActionGroupTest, group_preserves_action_order)
{
    PoseHandler p({2, -3, 'E'});
    ActionGroup{ActionType::TurnLeft, ActionType::Forward, ActionType::TurnLeft}.DoOperate(p);
    ASSERT_EQ((Pose{2, -2, 'W'}), p.Query());
}
TEST(ActionGroupTest, primitive_forward_does_not_apply_modes_twice)
{
    PoseHandler p({2, -3, 'E'}); p.Fast(); p.Reverse();
    ActionGroup{ActionType::Forward, ActionType::TurnLeft}.DoOperate(p);
    ASSERT_EQ((Pose{3, -3, 'N'}), p.Query());
}
TEST(ActionGroupTest, appended_actions_execute_in_order)
{
    PoseHandler p({0, 0, 'N'}); ActionGroup group;
    group.PushAction(ActionType::Forward); group.PushAction(ActionType::TurnRight);
    group.PushAction(ActionType::Forward); group.DoOperate(p);
    ASSERT_EQ((Pose{1, 1, 'E'}), p.Query());
}
}  // namespace adas
