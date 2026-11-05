#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"
#include "PoseEq.hpp"

namespace adas {
TEST(ExecutorTurnRoundTest, normal_tr_turns_left_moves_then_turns_left)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0, 0, 'E'}));
    p->Execute("TR");
    ASSERT_EQ((Pose{0, 1, 'W'}), p->Query());
}
TEST(ExecutorTurnRoundTest, fast_tr_adds_a_forward_step_before_turning)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0, 0, 'E'}));
    p->Execute("FTR");
    ASSERT_EQ((Pose{1, 1, 'W'}), p->Query());
}
TEST(ExecutorTurnRoundTest, reverse_ignores_the_whole_tr_command)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({0, 0, 'E'}));
    p->Execute("BTR");
    ASSERT_EQ((Pose{0, 0, 'E'}), p->Query());
}
}  // namespace adas
