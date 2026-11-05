#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"
#include "PoseEq.hpp"

namespace adas {
struct BusCase { const char* commands; Pose target; };
class BusStateTest : public testing::TestWithParam<BusCase> {};
TEST_P(BusStateTest, matches_course_rule_at_the_head_coordinate)
{
    const auto c=GetParam();
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'},VehicleType::Bus));
    ASSERT_NE(nullptr,p); p->Execute(c.commands); ASSERT_EQ(c.target,p->Query());
}
const BusCase busCases[] = {
    {"M",{3,-3,'E'}}, {"L",{3,-3,'N'}}, {"R",{3,-3,'S'}},
    {"BM",{1,-3,'E'}}, {"BL",{1,-3,'S'}}, {"BR",{1,-3,'N'}},
    {"FM",{4,-3,'E'}}, {"FL",{4,-3,'N'}}, {"FR",{4,-3,'S'}},
    {"BFM",{0,-3,'E'}}, {"BFL",{0,-3,'S'}}, {"BFR",{0,-3,'N'}},
};
INSTANTIATE_TEST_SUITE_P(FourStatesThreeCommands,BusStateTest,testing::ValuesIn(busCases));
}  // namespace adas
