#include "../include/fs.h"
#include <fstream>
#include <iostream>

namespace fs {

// ============================================================================
// Path Implementation
// ============================================================================

Path Path::from_parent(const std::string &parent,
                       const std::string &name) const {
  std::string combined = parent;
  if (!combined.empty() && combined.back() != '/') {
    combined += '/';
  }
  combined += name;
  return Path(combined);
}

Path::~Path() {}

// ============================================================================
// Dir Implementation
// ============================================================================

void Dir::add_subdir(Dir &&dir) noexcept {
  this->sub_dir_.push_back(std::move(dir));
}

void Dir::add_file(File &&file) noexcept {
  this->files_.push_back(std::move(file));
}

void Dir::write_to_disk() const {

  std::string dir_path = this->path_.to_string();
  // Create the directory
  if (mkdir(dir_path.c_str(), 0755) != 0) {
    if (errno != EEXIST) {
      throw std::runtime_error("Failed to create directory: " + dir_path);
    }
  }

  // Write all files to disk
  for (const auto &file : this->files_) {
    try {
      file.write_to_disk();
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }

  // Recursively write all sub-directories to disk
  for (const auto &sub_dir : this->sub_dir_) {
    try {
      sub_dir.write_to_disk();
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }
}

Dir::~Dir() {}

// ============================================================================
// File Implementation
// ============================================================================
void File::write_to_disk() const {
  std::string file_path = this->path_.to_string();
  std::ofstream file(file_path);

  if (!file) {
    throw std::runtime_error("Failed to create file: " + file_path);
  }

  file.write(this->content_.c_str(),
             static_cast<std::streamsize>(this->content_.size()));

  if (!file) {
    throw std::runtime_error("Failed to write complete content to file: " +
                             file_path);
  }
}

File::~File() {}

} // namespace fs
