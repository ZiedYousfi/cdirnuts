#include "../include/lua.h"

namespace Lua {
LuaEngine::LuaEngine() {
  lua_state_.open_libraries(sol::lib::base);
  registerAPI();
}

void LuaEngine::registerAPI() {
  auto cdirnuts = lua_state_.create_table("cdirnuts");

  cdirnuts["getCWD"] = []() { return std::filesystem::current_path(); };

  cdirnuts["create_virtual_dir"] = [](const std::string &path) {
    auto dir = new fs::Dir(path.c_str());
    if (!dir)
      throw std::runtime_error("Failed to create Dir");
    return dir;
  };

  cdirnuts["create_virtual_file"] = [](const std::string &name,
                                       const std::string &content) {
    auto file = new fs::File(name.c_str(), content.c_str());
    if (!file)
      throw std::runtime_error("Failed to create File");
    return file;
  };

  cdirnuts["write_virtual_file"] = [](const fs::File &file) {
    file.write_to_disk();
  };

  cdirnuts["write_virtual_dir"] = [](const fs::Dir &dir) {
    dir.write_to_disk();
  };

  cdirnuts["append_subdir"] = [](fs::Dir &parent, fs::Dir *child) {
    parent.add_subdir(child);
  };

  cdirnuts["append_file"] = [](fs::Dir &parent, fs::File *file) {
    parent.add_file(file);
  };
}

void LuaEngine::executeFile(const std::string &path) {
  lua_state_.script_file(path);
}

void LuaEngine::executeString(const std::string &code) {
  lua_state_.script(code);
}
} // namespace Lua
