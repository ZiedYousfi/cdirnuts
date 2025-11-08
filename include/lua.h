#pragma once

#include "fs.h"
#include <sol/sol.hpp>

namespace Lua {
class LuaEngine {
private:
  sol::state lua_state_;

public:
  LuaEngine() {}

  void registerAPI() {}

  void executeFile(const std::string &path) {}

  void executeString(const std::string &code) {}
};
} // namespace Lua
