#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "Executor.hpp"
#include "PoseEq.hpp"
#include "command/Command.hpp"
#include "command/CmderFactory.hpp"

namespace adas {
struct TurnRoundCase { char heading; const char* input; Pose target; };
class TurnRoundMatrixTest : public testing::TestWithParam<TurnRoundCase> {};
TEST_P(TurnRoundMatrixTest, matches_hand_calculated_pose)
{
    const auto c = GetParam();
    std::unique_ptr<Executor> p(Executor::NewExecutor({2, -3, c.heading}));
    p->Execute(c.input);
    ASSERT_EQ(c.target, p->Query());
}
const TurnRoundCase trCases[] = {
    {'E', "TR", {2,-2,'W'}}, {'E', "FTR", {3,-2,'W'}},
    {'E', "BTR", {2,-3,'E'}}, {'E', "FBTR", {2,-3,'E'}},
    {'S', "TR", {3,-3,'N'}}, {'S', "FTR", {3,-4,'N'}},
    {'S', "BTR", {2,-3,'S'}}, {'S', "FBTR", {2,-3,'S'}},
    {'W', "TR", {2,-4,'E'}}, {'W', "FTR", {1,-4,'E'}},
    {'W', "BTR", {2,-3,'W'}}, {'W', "FBTR", {2,-3,'W'}},
    {'N', "TR", {1,-3,'S'}}, {'N', "FTR", {1,-2,'S'}},
    {'N', "BTR", {2,-3,'N'}}, {'N', "FBTR", {2,-3,'N'}},
};
INSTANTIATE_TEST_SUITE_P(FourHeadingsFourStates, TurnRoundMatrixTest, testing::ValuesIn(trCases));

TEST(TurnRoundSequenceTest, adjacent_tokens_are_each_executed_once)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0,0,'E'}));
    p->Execute("TRTR"); ASSERT_EQ((Pose{0,0,'E'}),p->Query());
}
TEST(TurnRoundSequenceTest, r_after_tr_is_still_an_independent_right_turn)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0,0,'E'}));
    p->Execute("TRR"); ASSERT_EQ((Pose{0,1,'N'}),p->Query());
}
TEST(TurnRoundSequenceTest, toggles_are_observed_between_tokens)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0,0,'E'}));
    p->Execute("FTRFM"); ASSERT_EQ((Pose{0,1,'W'}),p->Query());
}
TEST(TurnRoundSequenceTest, ignored_tr_preserves_fast_and_reverse_states)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0,0,'E'}));
    p->Execute("FBTR"); ASSERT_EQ((Pose{0,0,'E'}),p->Query());
    p->Execute("BM"); ASSERT_EQ((Pose{2,0,'E'}),p->Query());
}
TEST(TurnRoundSequenceTest, tr_must_be_together_in_one_execute_call)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0,0,'E'}));
    p->Execute("T"); p->Execute("R"); ASSERT_EQ((Pose{0,0,'S'}),p->Query());
}
TEST(TurnRoundSequenceTest, trailing_t_is_not_consumed_as_a_turn_round)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0,0,'E'}));
    p->Execute("TRT"); ASSERT_EQ((Pose{0,1,'W'}),p->Query());
}
TEST(TurnRoundSequenceTest, internal_z_token_is_not_a_public_command)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0,0,'E'}));
    p->Execute("ZR"); ASSERT_EQ((Pose{0,0,'S'}),p->Query());
}
TEST(TurnRoundSequenceTest, plans_have_three_four_or_zero_primitive_actions)
{
    PoseHandler pose({0,0,'E'});
    ASSERT_EQ((std::vector<ActionType>{ActionType::TurnLeft, ActionType::Forward, ActionType::TurnLeft}),
              TurnRoundCommand{}(pose).Actions());
    pose.Fast();
    ASSERT_EQ((std::vector<ActionType>{ActionType::Forward,ActionType::TurnLeft,ActionType::Forward,ActionType::TurnLeft}),
              TurnRoundCommand{}(pose).Actions());
    pose.Reverse(); ASSERT_TRUE(TurnRoundCommand{}(pose).Actions().empty());
    ASSERT_EQ(4u, Singleton<CmderFactory>::Instance().GetCmders("MTRTRR").size());
}
}  // namespace adas
