#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"
#include "PoseEq.hpp"

namespace adas {
TEST(ExecutorBusTest, left_turn_moves_the_head_before_rotating)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'}, VehicleType::Bus));
    ASSERT_NE(nullptr, p);
    p->Execute("L"); ASSERT_EQ((Pose{3,-3,'N'}),p->Query());
}
TEST(ExecutorBusTest, right_turn_moves_the_head_before_rotating)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'}, VehicleType::Bus));
    ASSERT_NE(nullptr, p);
    p->Execute("R"); ASSERT_EQ((Pose{3,-3,'S'}),p->Query());
}
}  // namespace adas
