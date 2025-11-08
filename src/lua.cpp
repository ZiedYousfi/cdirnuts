#include "lua.h"

namespace Lua {
LuaEngine::LuaEngine() {
  lua.open_libraries(sol::lib::base);
  registerAPI();
}

void LuaEngine::registerAPI() {}

void LuaEngine::executeFile(const std::string &path) { lua.script_file(path); }

void LuaEngine::executeString(const std::string &code) { lua.script(code); }
} // namespace Lua
