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
  void use() const {}
  std::string get_name() const { return name_; }
  std::string get_path() const { return path_; }
  void print() const {
    std::cout << "Preset Name: " << name_ << ", Path: " << path_ << std::endl;
  }
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
  static PresetManager load_presets_from_file(const std::string &file_path);
};

} // namespace Presets
