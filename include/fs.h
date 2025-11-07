#pragma once

#include <string>
#include <vector>

namespace fs {

class Dir {
private:
  Path path_;
  std::vector<Dir> sub_dir_;

public:
  Dir() : path_("") {}
  Dir(const Path &path) : path_(path) {}
  Dir(const std::string &path) : path_(path) {}
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
