#include "../include/fs.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs {

// ============================================================================
// Path Implementation
// ============================================================================

Path Path::from_parent(const std::string &parent,
                       const std::string &name) const {
  std::filesystem::path combined(parent);
  combined /= name;
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

  std::filesystem::path dir_path = this->path_.to_path();

  // Create the directory and all parent directories if needed
  std::error_code ec;
  if (!std::filesystem::create_directories(dir_path, ec) && ec) {
    // Check if it failed for a reason other than already existing
    if (!std::filesystem::exists(dir_path)) {
      throw std::runtime_error("Failed to create directory: " +
                               dir_path.string() + " - " + ec.message());
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
  std::filesystem::path file_path = this->path_.to_path();
  std::ofstream file(file_path);

  if (!file) {
    throw std::runtime_error("Failed to create file: " + file_path.string());
  }

  file.write(this->content_.c_str(),
             static_cast<std::streamsize>(this->content_.size()));

  if (!file) {
    throw std::runtime_error("Failed to write complete content to file: " +
                             file_path.string());
  }
}

File::~File() {}

} // namespace fs
