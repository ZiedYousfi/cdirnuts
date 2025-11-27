#include "../include/presets.h"
#include "../include/lua.h"
#include <fstream>

namespace Presets {
// ============================================================================
// Preset Implementation
// ============================================================================

void Preset::use() const {
  Lua::LuaEngine lua;
  lua.execute_file(path_);
}

// ============================================================================
// PresetManager Implementation
// ============================================================================

void PresetManager::add_preset(const Preset &preset) {
  presets_.push_back(preset);
}

void PresetManager::remove_preset(const std::string &name) {
  presets_.erase(
      std::remove_if(presets_.begin(), presets_.end(),
                     [&name](const Preset &p) { return p.get_name() == name; }),
      presets_.end());
}

std::vector<Preset> PresetManager::list_presets() const { return presets_; }

Preset *PresetManager::get_preset(const std::string &name) {
  for (auto &preset : presets_) {
    if (preset.get_name() == name) {
      return &preset;
    }
  }
  return nullptr;
}

void PresetManager::save_presets_to_file(const std::string &file_path) const {
  std::ofstream file(file_path);
  if (!file) {
    std::cerr << "Error opening file for writing: " << file_path << std::endl;
    return;
  }
  for (const auto &preset : presets_) {
    file << "\"" << preset.get_name() << "\",\"" << preset.get_path() << "\"\n";
  }
}

PresetManager
PresetManager::load_presets_from_file(const std::string &file_path) {

  PresetManager result{};

  std::ifstream file(file_path);
  if (!file) {
    throw std::runtime_error("Error opening file for reading: " + file_path);
  }

  std::string line;
  while (std::getline(file, line)) {
    // Trim trailing '\r' if present (handles Windows CRLF line endings)
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    size_t comma_pos = line.find("\",\"");
    if (comma_pos != std::string::npos) {
      std::string name = line.substr(1, comma_pos - 1);
      std::string path =
          line.substr(comma_pos + 3, line.length() - comma_pos - 4);
      result.presets_.emplace_back(name, path);
    }
  }
  return result;
}

} // namespace Presets
