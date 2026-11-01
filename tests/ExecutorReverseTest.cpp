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

// B06: BFR 先倒退再左转
TEST(ExecutorReverseTest, moves_back_before_reverse_right)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BFR");
    const Pose target{1, -3, 'N'};
    ASSERT_EQ(target, executor->Query());
}

// B07: BBM 取消倒车
TEST(ExecutorReverseTest, cancels_reverse_after_two_toggles)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'N'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BBM");
    const Pose target{2, -2, 'N'};
    ASSERT_EQ(target, executor->Query());
}

// B08: FB 与 BF 状态正交
TEST(ExecutorReverseTest, commutes_fast_and_reverse_toggles)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    std::unique_ptr<Executor> other(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, other);
    executor->Execute("FBM");
    other->Execute("BFM");
    const Pose target{0, -3, 'E'};
    ASSERT_EQ(target, executor->Query());
    ASSERT_EQ(target, other->Query());
}

// B09: BM 西向后退一格
TEST(ExecutorReverseTest, moves_back_west)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'W'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BM");
    const Pose target{3, -3, 'W'};
    ASSERT_EQ(target, executor->Query());
}

// B10: BM 北向后退一格
TEST(ExecutorReverseTest, moves_back_north)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'N'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BM");
    const Pose target{2, -4, 'N'};
    ASSERT_EQ(target, executor->Query());
}

// B11: BM 南向后退一格
TEST(ExecutorReverseTest, moves_back_south)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'S'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BM");
    const Pose target{2, -2, 'S'};
    ASSERT_EQ(target, executor->Query());
}

// B12: 跨调用倒车保持与空输入
TEST(ExecutorReverseTest, preserves_reverse_across_calls)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("B");
    ASSERT_EQ((Pose{0, 0, 'N'}), executor->Query());
    executor->Execute("M");
    ASSERT_EQ((Pose{0, -1, 'N'}), executor->Query());
    executor->Execute("");
    ASSERT_EQ((Pose{0, -1, 'N'}), executor->Query());
    executor->Execute("L");
    ASSERT_EQ((Pose{0, -1, 'E'}), executor->Query());
    executor->Execute("M");
    ASSERT_EQ((Pose{-1, -1, 'E'}), executor->Query());
    executor->Execute("B");
    ASSERT_EQ((Pose{-1, -1, 'E'}), executor->Query());
    executor->Execute("M");
    ASSERT_EQ((Pose{0, -1, 'E'}), executor->Query());
}

// B13: 取消 F 后仍保持倒车
TEST(ExecutorReverseTest, keeps_reverse_when_fast_is_cancelled)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BFFM");
    const Pose target{1, -3, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// B14: 取消 B 后仍保持加速
TEST(ExecutorReverseTest, keeps_fast_when_reverse_is_cancelled)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'E'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("BFBM");
    const Pose target{4, -3, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// B15: 不同实例倒车状态隔离
TEST(ExecutorReverseTest, isolates_reverse_between_instances)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(nullptr, executor);
    std::unique_ptr<Executor> normal(Executor::NewExecutor());
    ASSERT_NE(nullptr, normal);
    executor->Execute("BM");
    normal->Execute("M");
    ASSERT_EQ((Pose{0, -1, 'N'}), executor->Query());
    ASSERT_EQ((Pose{0, 1, 'N'}), normal->Query());
}

// B16: BL 与 BR 四方向转向矩阵
TEST(ExecutorReverseTest, covers_reverse_turn_direction_matrix)
{
    struct Scenario { Pose initial; const char* commands; Pose target; };
    const Scenario scenarios[]{
        {{2, -3, 'E'}, "BL", {2, -3, 'S'}},
        {{2, -3, 'E'}, "BR", {2, -3, 'N'}},
        {{2, -3, 'S'}, "BL", {2, -3, 'W'}},
        {{2, -3, 'S'}, "BR", {2, -3, 'E'}},
        {{2, -3, 'W'}, "BL", {2, -3, 'N'}},
        {{2, -3, 'W'}, "BR", {2, -3, 'S'}},
        {{2, -3, 'N'}, "BL", {2, -3, 'E'}},
        {{2, -3, 'N'}, "BR", {2, -3, 'W'}},
    };
    for (const auto& scenario : scenarios) {
        SCOPED_TRACE(scenario.commands);
        SCOPED_TRACE(scenario.initial.heading);
        std::unique_ptr<Executor> executor(Executor::NewExecutor(scenario.initial));
        ASSERT_NE(nullptr, executor);
        executor->Execute(scenario.commands);
        ASSERT_EQ(scenario.target, executor->Query());
    }
}

// B17: BFM 其余三个方向
TEST(ExecutorReverseTest, covers_fast_reverse_move_directions)
{
    struct Scenario { Pose initial; const char* commands; Pose target; };
    const Scenario scenarios[]{
        {{2, -3, 'W'}, "BFM", {4, -3, 'W'}},
        {{2, -3, 'N'}, "BFM", {2, -5, 'N'}},
        {{2, -3, 'S'}, "BFM", {2, -1, 'S'}},
    };
    for (const auto& scenario : scenarios) {
        SCOPED_TRACE(scenario.commands);
        SCOPED_TRACE(scenario.initial.heading);
        std::unique_ptr<Executor> executor(Executor::NewExecutor(scenario.initial));
        ASSERT_NE(nullptr, executor);
        executor->Execute(scenario.commands);
        ASSERT_EQ(scenario.target, executor->Query());
    }
}

// B18: BFL 与 BFR 四方向矩阵
TEST(ExecutorReverseTest, covers_fast_reverse_turn_direction_matrix)
{
    struct Scenario { Pose initial; const char* commands; Pose target; };
    const Scenario scenarios[]{
        {{2, -3, 'E'}, "BFL", {1, -3, 'S'}},
        {{2, -3, 'E'}, "BFR", {1, -3, 'N'}},
        {{2, -3, 'S'}, "BFL", {2, -2, 'W'}},
        {{2, -3, 'S'}, "BFR", {2, -2, 'E'}},
        {{2, -3, 'W'}, "BFL", {3, -3, 'N'}},
        {{2, -3, 'W'}, "BFR", {3, -3, 'S'}},
        {{2, -3, 'N'}, "BFL", {2, -4, 'E'}},
        {{2, -3, 'N'}, "BFR", {2, -4, 'W'}},
    };
    for (const auto& scenario : scenarios) {
        SCOPED_TRACE(scenario.commands);
        SCOPED_TRACE(scenario.initial.heading);
        std::unique_ptr<Executor> executor(Executor::NewExecutor(scenario.initial));
        ASSERT_NE(nullptr, executor);
        executor->Execute(scenario.commands);
        ASSERT_EQ(scenario.target, executor->Query());
    }
}

// B19: 混合倒车轨迹与中间状态
TEST(ExecutorReverseTest, follows_reverse_route_step_by_step)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(nullptr, executor);
    std::unique_ptr<Executor> whole(Executor::NewExecutor());
    ASSERT_NE(nullptr, whole);
    whole->Execute("BFMLMRM");
    executor->Execute("B");
    ASSERT_EQ((Pose{0, 0, 'N'}), executor->Query());
    executor->Execute("F");
    ASSERT_EQ((Pose{0, 0, 'N'}), executor->Query());
    executor->Execute("M");
    ASSERT_EQ((Pose{0, -2, 'N'}), executor->Query());
    executor->Execute("L");
    ASSERT_EQ((Pose{0, -3, 'E'}), executor->Query());
    executor->Execute("M");
    ASSERT_EQ((Pose{-2, -3, 'E'}), executor->Query());
    executor->Execute("R");
    ASSERT_EQ((Pose{-3, -3, 'N'}), executor->Query());
    executor->Execute("M");
    ASSERT_EQ((Pose{-3, -5, 'N'}), executor->Query());
    ASSERT_EQ(whole->Query(), executor->Query());
}

// B20: 四次倒车转向与取消恢复
TEST(ExecutorReverseTest, restores_after_full_reverse_turn_cycle)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({2, -3, 'N'}));
    ASSERT_NE(nullptr, executor);
    executor->Execute("B");
    for (const Pose target : {Pose{2, -3, 'E'}, Pose{2, -3, 'S'},
                              Pose{2, -3, 'W'}, Pose{2, -3, 'N'}}) {
        executor->Execute("L");
        ASSERT_EQ(target, executor->Query());
    }
    executor->Execute("BM");
    ASSERT_EQ((Pose{2, -2, 'N'}), executor->Query());
}
}  // namespace adas
