#include "command/CmderFactory.hpp"
#include "command/Command.hpp"
#include "command/VehicleCommand.hpp"

namespace adas {
namespace {
CmderMap MakeBusMap(const CmderMap& standard)
{
    auto result = standard;
    result.at('L') = BusTurnCommand{true};
    result.at('R') = BusTurnCommand{false};
    return result;
}
CmderMap MakeSportsMap(const CmderMap& standard)
{
    auto result = standard;
    result.at('M') = SportsMoveCommand{};
    result.at('L') = SportsTurnCommand{true};
    result.at('R') = SportsTurnCommand{false};
    return result;
}
}
CmderFactory::CmderFactory()
    : cmderMap{{'M', MoveCommand{}}, {'L', TurnLeftCommand{}}, {'R', TurnRightCommand{}},
               {'F', FastCommand{}}, {'B', ReverseCommand{}}, {'Z', TurnRoundCommand{}}},
      busMap(MakeBusMap(cmderMap)), sportsMap(MakeSportsMap(cmderMap))
{
}
CmderList CmderFactory::GetCmders(const std::string& commands, VehicleType type) const noexcept
{
    const auto& selected = type == VehicleType::Bus ? busMap :
                           type == VehicleType::SportsCar ? sportsMap : cmderMap;
    CmderList result;
    for (const char command : ParseCommandString(commands)) {
        const auto it = selected.find(command);
        if (it != selected.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}
std::string CmderFactory::ParseCommandString(const std::string& commands) const
{
    std::string result;
    result.reserve(commands.size());
    for (std::size_t i = 0; i < commands.size(); ++i) {
        if (commands[i] == 'T' && i + 1 < commands.size() && commands[i + 1] == 'R') {
            result += 'Z';  // Internal token for the two-character TR instruction.
            ++i;
        } else if (commands[i] != 'Z') {
            result += commands[i];  // A literal Z must not become an accidental public command.
        }
    }
    return result;
}
}  // namespace adas
