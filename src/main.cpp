#include "presets.h"
#include <CLI/CLI.hpp>
#include <iostream>

/**
 * Program entry point that parses command-line options, performs default
 * initialization, and returns an exit status.
 *
 * Supported options:
 * - --help: prints usage and exits.
 * - --config <file>: loads configuration from <file>
 * - --preset <name>: loads a preset configuration by name
 * - --preset list: lists all saved presets
 * - --preset add <name> <path>: adds a new preset
 * - --preset remove <name>: removes a preset by name
 */
int main(int argc, char **argv) {

  Presets::PresetManager preset_manager;

  auto file_path =
      std::getenv("DIRNUTS_DIR_PATH")
          ? std::string(std::getenv("DIRNUTS_DIR_PATH")) + "/presets.cdndb"
          : "presets.cdndb";

  try {
    preset_manager.load_presets_from_file(file_path);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  CLI::App app("cdirnuts - Project initialization tool");

  int result = 0;

  // --config option
  auto *config_cmd =
      app.add_subcommand("config", "Specify a configuration file");
  std::string config_file;
  config_cmd->add_option("file", config_file, "Configuration file path")
      ->required();
  config_cmd->callback([&]() {
    // TODO: Implement configuration file loading
  });

  // --preset subcommand with nested options
  auto *preset_cmd = app.add_subcommand("preset", "Manage presets");

  // --preset list
  auto *preset_list =
      preset_cmd->add_subcommand("list", "List all saved presets");
  preset_list->callback([&]() {
    auto presets = preset_manager.list_presets();
    if (presets.empty()) {
      std::cout << "No presets saved.\n";
    } else {
      for (const auto &preset : presets) {
        preset.print();
      }
    }
  });

  // --preset add <name> <path>
  auto *preset_add = preset_cmd->add_subcommand("add", "Add a new preset");
  std::string preset_add_name, preset_add_path;
  preset_add->add_option("name", preset_add_name, "Preset name")->required();
  preset_add->add_option("path", preset_add_path, "Preset path")->required();
  preset_add->callback([&]() {
    Presets::Preset new_preset(preset_add_name, preset_add_path);
    preset_manager.add_preset(new_preset);
    try {
      preset_manager.save_presets_to_file(file_path);
      std::cout << "Preset added successfully.\n";
    } catch (const std::exception &e) {
      std::cerr << "Error saving presets: " << e.what() << '\n';
    }
  });

  // --preset remove <name>
  auto *preset_remove = preset_cmd->add_subcommand("remove", "Remove a preset");
  std::string preset_remove_name;
  preset_remove->add_option("name", preset_remove_name, "Preset name")
      ->required();
  preset_remove->callback([&]() {
    preset_manager.remove_preset(preset_remove_name);
    try {
      preset_manager.save_presets_to_file(file_path);
      std::cout << "Preset removed successfully.\n";
    } catch (const std::exception &e) {
      std::cerr << "Error saving presets: " << e.what() << '\n';
    }
  });

  // --preset <name> (use preset directly)
  auto *preset_use = preset_cmd->add_subcommand("use", "Use a preset by name");
  std::string preset_use_name;
  preset_use->add_option("name", preset_use_name, "Preset name")->required();
  preset_use->callback([&]() {
    // TODO: Implement using a preset
  });

  // Default behavior (no args)
  app.callback([&]() {
    if (!*config_cmd && !*preset_cmd) {
      // TODO: Implement default behavior
    }
  });

  try {
    CLI11_PARSE(app, argc, argv);
  } catch (const CLI::ParseError &e) {
    std::cerr << e.what() << std::endl;
    return app.exit(e);
  }

  return result;
}
