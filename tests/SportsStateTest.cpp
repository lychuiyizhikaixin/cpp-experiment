#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"
#include "PoseEq.hpp"

namespace adas {
struct SportsCase { const char* commands; Pose target; };
class SportsStateTest : public testing::TestWithParam<SportsCase> {};
TEST_P(SportsStateTest, matches_course_movement_and_turn_order)
{
    const auto c=GetParam();
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'},VehicleType::SportsCar));
    ASSERT_NE(nullptr,p); p->Execute(c.commands); ASSERT_EQ(c.target,p->Query());
}
const SportsCase sportsCases[] = {
    {"M",{4,-3,'E'}}, {"L",{2,-2,'N'}}, {"R",{2,-4,'S'}},
    {"BM",{0,-3,'E'}}, {"BL",{2,-2,'S'}}, {"BR",{2,-4,'N'}},
    {"FM",{6,-3,'E'}}, {"FL",{3,-2,'N'}}, {"FR",{3,-4,'S'}},
    {"BFM",{-2,-3,'E'}}, {"BFL",{1,-2,'S'}}, {"BFR",{1,-4,'N'}},
};
INSTANTIATE_TEST_SUITE_P(FourStatesThreeCommands,SportsStateTest,testing::ValuesIn(sportsCases));
}  // namespace adas
