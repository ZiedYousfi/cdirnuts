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

void Dir::AddSubDir(const Dir &dir) {
  Dir *new_sub_dirs = new Dir[sub_dir_count_ + 1];

  // Copy existing subdirectories
  for (size_t i = 0; i < sub_dir_count_; ++i) {
    new_sub_dirs[i] = sub_dir_[i];
  }

  // Add the new subdirectory
  new_sub_dirs[sub_dir_count_] = dir;

  // Clean up old array
  delete[] sub_dir_;

  // Update pointers
  sub_dir_ = new_sub_dirs;
  ++sub_dir_count_;
}

Dir::~Dir() { delete[] sub_dir_; }

// ============================================================================
// File Implementation
// ============================================================================

File::~File() {}

} // namespace fs
