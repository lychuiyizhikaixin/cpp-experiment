#pragma once
#include "PoseHandler.hpp"
#include "Singleton.hpp"
#include <functional>
#include <list>
#include <string>
#include <unordered_map>

namespace adas {
using Cmder = std::function<void(PoseHandler&)>;
using CmderList = std::list<Cmder>;
class CmderFactory final {
public:
    CmderList GetCmders(const std::string& commands) const noexcept;
    CmderFactory(const CmderFactory&) = delete;
    CmderFactory& operator=(const CmderFactory&) = delete;
private:
    friend class Singleton<CmderFactory>;
    CmderFactory();
    const std::unordered_map<char, Cmder> cmderMap;
};
}  // namespace adas
