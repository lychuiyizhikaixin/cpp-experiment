#include <gtest/gtest.h>
#include "../src/CmderFactory.hpp"
#include "PoseEq.hpp"

namespace adas {
TEST(CmderFactoryTest, singleton_returns_the_same_factory)
{
    ASSERT_EQ(&Singleton<CmderFactory>::Instance(), &Singleton<CmderFactory>::Instance());
}
TEST(CmderFactoryTest, preserves_command_order)
{
    PoseHandler pose({0, 0, 'N'});
    for (const auto& cmd : Singleton<CmderFactory>::Instance().GetCmders("MRM")) { cmd(pose).DoOperate(pose); }
    ASSERT_EQ((Pose{1, 1, 'E'}), pose.Query());
}
TEST(CmderFactoryTest, commands_read_state_at_execution_time)
{
    PoseHandler pose({0, 0, 'N'});
    const auto cmds = Singleton<CmderFactory>::Instance().GetCmders("FBM");
    for (const auto& cmd : cmds) { cmd(pose).DoOperate(pose); }
    ASSERT_EQ((Pose{0, -2, 'N'}), pose.Query());
}
TEST(CmderFactoryTest, empty_input_returns_an_empty_list)
{
    ASSERT_TRUE(Singleton<CmderFactory>::Instance().GetCmders("").empty());
}
TEST(CmderFactoryTest, one_command_list_can_operate_on_independent_receivers)
{
    PoseHandler first({0, 0, 'N'}), second({2, 3, 'E'});
    const auto cmds = Singleton<CmderFactory>::Instance().GetCmders("FM");
    for (const auto& cmd : cmds) { cmd(first).DoOperate(first); cmd(second).DoOperate(second); }
    ASSERT_EQ((Pose{0, 2, 'N'}), first.Query());
    ASSERT_EQ((Pose{4, 3, 'E'}), second.Query());
}
}  // namespace adas
