#include "../include/fs.h"
#include <algorithm>
#include <stdexcept>

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

Dir::~Dir() {}

// ============================================================================
// File Implementation
// ============================================================================

File::~File() {}

} // namespace fs
