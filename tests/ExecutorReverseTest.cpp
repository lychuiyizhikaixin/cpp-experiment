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
}  // namespace adas
