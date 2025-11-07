#include "../include/fs.h"
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

void Dir::AddSubDir(Dir *dir) {
  this->sub_dir_.push_back(*dir);
  dir = nullptr;
}

void Dir::AddFile(File *file) {
  this->files_.push_back(*file);
  file = nullptr;
}

void Dir::WriteToDisk() const {

  std::string dir_path = this->path_.to_string();
  // Create the directory
  if (mkdir(dir_path.c_str(), 0755) != 0) {
    throw std::runtime_error("Failed to create directory: " + dir_path);
  }

  // Write all files to disk
  for (const auto &file : this->files_) {
    try {
      file.WriteToDisk();
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }

  // Recursively write all sub-directories to disk
  for (const auto &sub_dir : this->sub_dir_) {
    try {
      sub_dir.WriteToDisk();
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }
}

Dir::~Dir() {}

// ============================================================================
// File Implementation
// ============================================================================

void File::WriteToDisk() const {
  std::string file_path = this->path_.to_string();
  FILE *file = fopen(file_path.c_str(), "w");

  if (!file) {
    throw std::runtime_error("Failed to create file: " + file_path);
  }

  fwrite(this->content_.c_str(), sizeof(char), this->content_.size(), file);
  fclose(file);
}

File::~File() {}

} // namespace fs
