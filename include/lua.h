#pragma once

#include <sol/sol.hpp>
#include <string>

namespace Lua {
class LuaEngine {
private:
  sol::state lua_state_;

public:
  LuaEngine();

  void register_api();

  void execute_file(const std::string &path);

  void execute_string(const std::string &code);
};
} // namespace Lua
