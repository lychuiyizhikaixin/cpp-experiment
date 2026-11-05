#include "CmderFactory.hpp"
#include "Command.hpp"

namespace adas {
CmderFactory::CmderFactory()
    : cmderMap{{'M', MoveCommand{}}, {'L', TurnLeftCommand{}}, {'R', TurnRightCommand{}},
               {'F', FastCommand{}}, {'B', ReverseCommand{}}}
{
}
CmderList CmderFactory::GetCmders(const std::string& commands) const noexcept
{
    CmderList result;
    for (const char command : commands) {
        const auto it = cmderMap.find(command);
        if (it != cmderMap.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}
}  // namespace adas
