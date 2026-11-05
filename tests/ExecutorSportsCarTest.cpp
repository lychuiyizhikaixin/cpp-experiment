#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"
#include "PoseEq.hpp"

namespace adas {
TEST(ExecutorSportsCarTest, normal_move_runs_two_grids)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'},VehicleType::SportsCar));
    ASSERT_NE(nullptr,p); p->Execute("M"); ASSERT_EQ((Pose{4,-3,'E'}),p->Query());
}
TEST(ExecutorSportsCarTest, normal_left_rotates_before_moving)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'},VehicleType::SportsCar));
    ASSERT_NE(nullptr,p); p->Execute("L"); ASSERT_EQ((Pose{2,-2,'N'}),p->Query());
}
}  // namespace adas
