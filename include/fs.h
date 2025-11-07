#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace fs {

class Dir {
private:
  Path path_;
  std::vector<Dir> sub_dir_;
  std::vector<File> files_;

public:
  Dir() : path_("") {}
  Dir(const Path &path) : path_(path) {}
  Dir(const std::string &path) : path_(path) {}
  /// @brief Add a sub-directory to the current directory. Takes ownership.
  /// @param dir
  void AddSubDir(Dir *dir) noexcept;
  /// @brief Add a file to the current directory. Takes ownership.
  /// @param file
  void AddFile(File *file) noexcept;
  void WriteToDisk() const;
  ~Dir();
};

class File {
private:
  Path path_;
  std::string content_;

public:
  File() : path_(""), content_("") {}
  File(const Path &path, const std::string &content)
      : path_(path), content_(content) {}
  File(const std::string &path, const std::string &content)
      : path_(path), content_(content) {}
  File(const Path &path) : path_(path), content_("") {}
  File(const std::string &path) : path_(path), content_("") {}
  void WriteToDisk() const {}
  ~File();
};

class Path {
private:
  std::string path_;

public:
  Path() : path_("") {}
  Path(const std::string &path) : path_(path) {}
  Path from_parent(const std::string &parent, const std::string &name) const {}
  ~Path();
  std::string to_string() const { return path_; }
};

} // namespace fs
