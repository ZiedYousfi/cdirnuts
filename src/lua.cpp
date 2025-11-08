#include "../include/lua.h"

namespace Lua {
LuaEngine::LuaEngine() {
  lua_state_.open_libraries(sol::lib::base, sol::lib::io, sol::lib::string);
  register_api();
}

void LuaEngine::register_api() {
  auto cdirnuts = lua_state_.create_table("cdirnuts");

  cdirnuts["getCWD"] = []() {
    return std::filesystem::current_path().string();
  };

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

  cdirnuts["execute_shell_command"] = [](const std::string &command) {
    std::cout << "Execute command: " << command << "? (y/n): ";
    std::string response;
    std::getline(std::cin, response);
    if (response != "y" && response != "Y") {
      throw std::runtime_error("Command execution cancelled by user");
    }

    int ret = std::system(command.c_str());
    if (ret != 0) {
      throw std::runtime_error("Shell command failed with exit code " +
                               std::to_string(ret));
    }
  };
}

void LuaEngine::execute_file(const std::string &path) {
  lua_state_.script_file(path);
}

void LuaEngine::execute_string(const std::string &code) {
  lua_state_.script(code);
}
} // namespace Lua
