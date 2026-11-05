#include <gtest/gtest.h>
#include <memory>
#include "data/VehicleCases.hpp"
#include "PoseEq.hpp"

namespace adas {
class VehicleMatrixTest : public testing::TestWithParam<VehicleCase> {};
TEST_P(VehicleMatrixTest, matches_hand_calculated_direction_and_state)
{
    const auto c=GetParam();
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,c.heading},c.type));
    ASSERT_NE(nullptr,p); p->Execute(c.commands); ASSERT_EQ(c.target,p->Query());
}
INSTANTIATE_TEST_SUITE_P(TwoVehiclesFourHeadingsFourStates,VehicleMatrixTest,testing::ValuesIn(vehicleCases));

struct VehicleTrCase { VehicleType type; const char* commands; Pose target; };
class VehicleTurnRoundTest : public testing::TestWithParam<VehicleTrCase> {};
TEST_P(VehicleTurnRoundTest, reuses_the_common_atomic_turn_round_rule)
{
    const auto c=GetParam();
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'},c.type));
    p->Execute(c.commands); ASSERT_EQ(c.target,p->Query());
}
const VehicleTrCase vehicleTrCases[] = {
    {VehicleType::Bus,"TR",{2,-2,'W'}}, {VehicleType::Bus,"FTR",{3,-2,'W'}},
    {VehicleType::Bus,"BTR",{2,-3,'E'}}, {VehicleType::Bus,"BFTR",{2,-3,'E'}},
    {VehicleType::SportsCar,"TR",{2,-2,'W'}}, {VehicleType::SportsCar,"FTR",{3,-2,'W'}},
    {VehicleType::SportsCar,"BTR",{2,-3,'E'}}, {VehicleType::SportsCar,"BFTR",{2,-3,'E'}},
};
INSTANTIATE_TEST_SUITE_P(SharedAtomicRule,VehicleTurnRoundTest,testing::ValuesIn(vehicleTrCases));

TEST(VehicleSequenceTest, vehicle_modes_are_kept_in_individual_receivers)
{
    std::unique_ptr<Executor> normal(Executor::NewExecutor({0,0,'E'}));
    std::unique_ptr<Executor> bus(Executor::NewExecutor({0,0,'E'},VehicleType::Bus));
    std::unique_ptr<Executor> sports(Executor::NewExecutor({0,0,'E'},VehicleType::SportsCar));
    normal->Execute("FM"); bus->Execute("BM"); sports->Execute("FBM");
    ASSERT_EQ((Pose{2,0,'E'}),normal->Query());
    ASSERT_EQ((Pose{-1,0,'E'}),bus->Query());
    ASSERT_EQ((Pose{-4,0,'E'}),sports->Query());
    bus->Execute("BM"); normal->Execute("M"); sports->Execute("M");
    ASSERT_EQ((Pose{0,0,'E'}),bus->Query());
    ASSERT_EQ((Pose{4,0,'E'}),normal->Query());
    ASSERT_EQ((Pose{-8,0,'E'}),sports->Query());
}
TEST(VehicleSequenceTest, bus_keeps_modes_between_calls_and_cancels_them_independently)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'},VehicleType::Bus));
    p->Execute("FBL"); ASSERT_EQ((Pose{0,-3,'S'}),p->Query());
    p->Execute("M"); ASSERT_EQ((Pose{0,-1,'S'}),p->Query());
    p->Execute("BFLM"); ASSERT_EQ((Pose{1,-2,'E'}),p->Query());
}
TEST(VehicleSequenceTest, sports_keeps_post_turn_movement_in_the_new_heading)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'},VehicleType::SportsCar));
    p->Execute("FB"); p->Execute("L"); ASSERT_EQ((Pose{1,-2,'S'}),p->Query());
    p->Execute("FM"); ASSERT_EQ((Pose{1,0,'S'}),p->Query());
    p->Execute("BR"); ASSERT_EQ((Pose{0,0,'W'}),p->Query());
}
TEST(VehicleSequenceTest, old_factory_signature_still_selects_normal_vehicle)
{
    std::unique_ptr<Executor> old(Executor::NewExecutor({2,-3,'E'}));
    std::unique_ptr<Executor> typed(Executor::NewExecutor({2,-3,'E'},VehicleType::Normal));
    old->Execute("FMTRBMLFBM"); typed->Execute("FMTRBMLFBM");
    ASSERT_EQ(old->Query(),typed->Query());
}
TEST(VehicleSequenceTest, empty_calls_do_not_reset_sports_modes)
{
    std::unique_ptr<Executor> p(Executor::NewExecutor({2,-3,'E'},VehicleType::SportsCar));
    p->Execute("BF"); p->Execute(""); p->Execute("M");
    ASSERT_EQ((Pose{-2,-3,'E'}),p->Query());
    p->Execute("BM"); ASSERT_EQ((Pose{2,-3,'E'}),p->Query());
}
}  // namespace adas
