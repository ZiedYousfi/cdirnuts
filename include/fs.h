#pragma once

#include <string>

namespace fs {

class Dir {
private:
  Path path_;
  Dir *sub_dir_;
  size_t sub_dir_count_;

public:
  Dir() : path_(""), sub_dir_(nullptr), sub_dir_count_(0) {}
  Dir(const Path &path) : path_(path), sub_dir_(nullptr), sub_dir_count_(0) {}
  Dir(const std::string &path)
      : path_(path), sub_dir_(nullptr), sub_dir_count_(0) {}
  void AddSubDir(const Dir &dir);
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
