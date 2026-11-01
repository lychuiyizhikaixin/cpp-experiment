#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"
#include "PoseEq.hpp"

namespace adas {

// B01: BM 东向后退一格
TEST(ExecutorReverseTest, moves_back_east)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BM");
    const Pose target{1, -3, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// B02: BL 东向反向右转
TEST(ExecutorReverseTest, turns_right_for_reverse_left)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BL");
    const Pose target{2, -3, 'S'};
    ASSERT_EQ(target, executor->Query());
}

// B03: BR 东向反向左转
TEST(ExecutorReverseTest, turns_left_for_reverse_right)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BR");
    const Pose target{2, -3, 'N'};
    ASSERT_EQ(target, executor->Query());
}

// B04: BFM 加速倒退两格
TEST(ExecutorReverseTest, moves_back_twice_when_fast)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BFM");
    const Pose target{0, -3, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// B05: BFL 先倒退再右转
TEST(ExecutorReverseTest, moves_back_before_reverse_left)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BFL");
    const Pose target{1, -3, 'S'};
    ASSERT_EQ(target, executor->Query());
}
}  // namespace adas
