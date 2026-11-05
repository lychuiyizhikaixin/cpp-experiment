#include "CmderFactory.hpp"
#include "Command.hpp"

namespace adas {
CmderFactory::CmderFactory()
    : cmderMap{{'M', MoveCommand{}}, {'L', TurnLeftCommand{}}, {'R', TurnRightCommand{}},
               {'F', FastCommand{}}, {'B', ReverseCommand{}}, {'Z', TurnRoundCommand{}}}
{
}
CmderList CmderFactory::GetCmders(const std::string& commands) const noexcept
{
    CmderList result;
    for (const char command : ParseCommandString(commands)) {
        const auto it = cmderMap.find(command);
        if (it != cmderMap.end()) {
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
