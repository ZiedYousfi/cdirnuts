#include "../include/lua.h"
#include "./fs.h"
#include <filesystem>
#include <iostream>
#include <memory>

namespace Lua {
LuaEngine::LuaEngine() {
  lua_state_.open_libraries(sol::lib::base, sol::lib::io, sol::lib::string);
  register_api();
}

void LuaEngine::register_api() {
  // Register usertypes for fs::Dir and fs::File to enable Sol2 to handle
  // shared_ptr instances
  lua_state_.new_usertype<fs::Dir>("Dir");
  lua_state_.new_usertype<fs::File>("File");

  auto cdirnuts = lua_state_.create_table("cdirnuts");

  cdirnuts["getCWD"] = []() {
    return std::filesystem::current_path().string();
  };

  // Modified factories to return std::shared_ptr for managed ownership
  cdirnuts["create_virtual_dir"] =
      [](const std::string &path) -> std::shared_ptr<fs::Dir> {
    return std::make_shared<fs::Dir>(path);
  };

  cdirnuts["create_virtual_file"] =
      [](const std::string &name,
         const std::string &content) -> std::shared_ptr<fs::File> {
    return std::make_shared<fs::File>(name, content);
  };

  // Updated bindings to accept std::shared_ptr for consistency
  cdirnuts["write_virtual_file"] = [](std::shared_ptr<fs::File> file) {
    file->write_to_disk();
  };

  cdirnuts["write_virtual_dir"] = [](std::shared_ptr<fs::Dir> dir) {
    dir->write_to_disk();
  };

  cdirnuts["append_subdir"] = [](std::shared_ptr<fs::Dir> parent,
                                 std::shared_ptr<fs::Dir> child) {
    parent->add_subdir(std::move(*child));
  };

  cdirnuts["append_file"] = [](std::shared_ptr<fs::Dir> parent,
                               std::shared_ptr<fs::File> file) {
    parent->add_file(std::move(*file));
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
