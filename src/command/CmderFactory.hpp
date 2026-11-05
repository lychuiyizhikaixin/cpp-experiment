#pragma once
#include "action/ActionGroup.hpp"
#include "common/Singleton.hpp"
#include <functional>
#include <list>
#include <string>
#include <unordered_map>

namespace adas {
using Cmder = std::function<ActionGroup(PoseHandler&)>;
using CmderList = std::list<Cmder>;
using CmderMap = std::unordered_map<char, Cmder>;
class CmderFactory final {
public:
    CmderList GetCmders(const std::string& commands, VehicleType type = VehicleType::Normal) const noexcept;
    CmderFactory(const CmderFactory&) = delete;
    CmderFactory& operator=(const CmderFactory&) = delete;
private:
    friend class Singleton<CmderFactory>;
    CmderFactory();
    std::string ParseCommandString(const std::string& commands) const;
    const CmderMap cmderMap;
    const CmderMap busMap;
    const CmderMap sportsMap;
};
}  // namespace adas
