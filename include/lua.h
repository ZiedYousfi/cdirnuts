#pragma once

#include "fs.h"
#include <sol/sol.hpp>

namespace Lua {
class LuaEngine {
private:
  sol::state lua;

public:
  LuaEngine() {
    lua.open_libraries(sol::lib::base);
    registerAPI();
  }

  void registerAPI() {
    auto cdirnuts = lua.create_table("cdirnuts");

    // TODO: Implement API functions
  }

  void executeFile(const std::string &path) { lua.script_file(path); }

  void executeString(const std::string &code) { lua.script(code); }
};
} // namespace Lua
