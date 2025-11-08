#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Presets {

class Preset {
private:
  std::string name_;
  std::string path_;

public:
  Preset() = default;
  Preset(const std::string &name, const std::string &path)
      : name_(name), path_(path) {}
  std::string get_name() const { return name_; }
  std::string get_path() const { return path_; }
};

class PresetManager {
private:
  std::vector<Preset> presets_;

public:
  void add_preset(const Preset &preset);
  void remove_preset(const std::string &name);
  std::vector<Preset> list_presets() const;
  Preset *get_preset(const std::string &name);
  void save_presets_to_file(const std::string &file_path) const;
  void load_presets_from_file(const std::string &file_path);
};

} // namespace Presets
