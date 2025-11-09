#include "../include/fs.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace fs_test {

// Test fixture for file system tests
class FsTest : public ::testing::Test {
protected:
  std::string test_dir = "./test_output";

  void SetUp() override {
    // Clean up test directory before each test
    std::filesystem::remove_all(test_dir);
  }

  void TearDown() override {
    // Clean up test directory after each test
    std::filesystem::remove_all(test_dir);
  }

  bool file_exists(const std::string &path) {
    return std::filesystem::exists(path);
  }

  std::string read_file(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
      return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
  }
};

// ============================================================================
// Path Tests
// ============================================================================

TEST_F(FsTest, PathConstruction) {
  fs::Path path1;
  EXPECT_EQ(path1.to_string(), "");

  fs::Path path2("test/path");
  EXPECT_EQ(path2.to_string(), "test/path");
}

TEST_F(FsTest, PathFromParent) {
  fs::Path path;
  fs::Path child = path.from_parent("parent", "child");
  EXPECT_EQ(child.to_string(), "parent/child");

  fs::Path child_with_slash = path.from_parent("parent/", "child");
  EXPECT_EQ(child_with_slash.to_string(), "parent/child");
}

// ============================================================================
// File Tests
// ============================================================================

TEST_F(FsTest, FileConstruction) {
  fs::File file1;

  fs::File file2("test.txt", "content");

  fs::Path path("test.txt");
  fs::File file3(path, "content");

  // No exceptions thrown
  SUCCEED();
}

TEST_F(FsTest, FileWriteToDisk) {
  std::filesystem::create_directories(test_dir);

  std::string file_path = test_dir + "/test_file.txt";
  std::string content = "Hello, World!";

  fs::File file(file_path, content);
  file.write_to_disk();

  EXPECT_TRUE(file_exists(file_path));
  EXPECT_EQ(read_file(file_path), content);
}

TEST_F(FsTest, FileWriteEmptyContent) {
  std::filesystem::create_directories(test_dir);

  std::string file_path = test_dir + "/empty_file.txt";
  fs::File file(file_path, "");
  file.write_to_disk();

  EXPECT_TRUE(file_exists(file_path));
  EXPECT_EQ(read_file(file_path), "");
}

TEST_F(FsTest, FileWriteMultilineContent) {
  std::filesystem::create_directories(test_dir);

  std::string file_path = test_dir + "/multiline.txt";
  std::string content = "Line 1\nLine 2\nLine 3\n";

  fs::File file(file_path, content);
  file.write_to_disk();

  EXPECT_TRUE(file_exists(file_path));
  EXPECT_EQ(read_file(file_path), content);
}

// ============================================================================
// Dir Tests
// ============================================================================

TEST_F(FsTest, DirConstruction) {
  fs::Dir dir1;

  fs::Dir dir2("test_dir");

  fs::Path path("test_dir");
  fs::Dir dir3(path);

  // No exceptions thrown
  SUCCEED();
}

TEST_F(FsTest, DirWriteToDisk) {
  std::filesystem::create_directories(test_dir);
  std::string dir_path = test_dir + "/new_dir";

  fs::Dir dir(dir_path);
  dir.write_to_disk();

  EXPECT_TRUE(std::filesystem::exists(dir_path));
  EXPECT_TRUE(std::filesystem::is_directory(dir_path));
}

TEST_F(FsTest, DirWithFiles) {
  std::filesystem::create_directories(test_dir);
  std::string dir_path = test_dir + "/dir_with_files";

  fs::Dir dir(dir_path);

  auto file1 = fs::File(dir_path + "/file1.txt", "Content 1");
  auto file2 = fs::File(dir_path + "/file2.txt", "Content 2");

  dir.add_file(std::move(file1));
  dir.add_file(std::move(file2));

  dir.write_to_disk();

  EXPECT_TRUE(file_exists(dir_path + "/file1.txt"));
  EXPECT_TRUE(file_exists(dir_path + "/file2.txt"));
  EXPECT_EQ(read_file(dir_path + "/file1.txt"), "Content 1");
  EXPECT_EQ(read_file(dir_path + "/file2.txt"), "Content 2");
}

TEST_F(FsTest, DirWithSubdirs) {
  std::filesystem::create_directories(test_dir);
  std::string dir_path = test_dir + "/parent_dir";

  fs::Dir dir(dir_path);

  auto subdir1 = fs::Dir(dir_path + "/subdir1");
  auto subdir2 = fs::Dir(dir_path + "/subdir2");

  dir.add_subdir(std::move(subdir1));
  dir.add_subdir(std::move(subdir2));

  dir.write_to_disk();

  EXPECT_TRUE(std::filesystem::exists(dir_path + "/subdir1"));
  EXPECT_TRUE(std::filesystem::is_directory(dir_path + "/subdir1"));
  EXPECT_TRUE(std::filesystem::exists(dir_path + "/subdir2"));
  EXPECT_TRUE(std::filesystem::is_directory(dir_path + "/subdir2"));
}

TEST_F(FsTest, NestedDirStructure) {
  std::filesystem::create_directories(test_dir);
  std::string dir_path = test_dir + "/root";

  fs::Dir root(dir_path);

  // Create nested structure
  auto subdir = fs::Dir(dir_path + "/subdir");
  auto file_in_root = fs::File(dir_path + "/root.txt", "Root file");
  auto file_in_subdir = fs::File(dir_path + "/subdir/sub.txt", "Subdir file");

  subdir.add_file(std::move(file_in_subdir));
  root.add_subdir(std::move(subdir));
  root.add_file(std::move(file_in_root));

  root.write_to_disk();

  EXPECT_TRUE(file_exists(dir_path + "/root.txt"));
  EXPECT_TRUE(file_exists(dir_path + "/subdir/sub.txt"));
  EXPECT_EQ(read_file(dir_path + "/root.txt"), "Root file");
  EXPECT_EQ(read_file(dir_path + "/subdir/sub.txt"), "Subdir file");
}

TEST_F(FsTest, ComplexDirStructure) {
  std::filesystem::create_directories(test_dir);
  std::string dir_path = test_dir + "/complex";

  fs::Dir root(dir_path);

  // Create a complex structure: root/src, root/include, root/tests
  auto src_dir = fs::Dir(dir_path + "/src");
  auto include_dir = fs::Dir(dir_path + "/include");
  auto tests_dir = fs::Dir(dir_path + "/tests");

  auto main_cpp = fs::File(dir_path + "/src/main.cpp", "int main() {}");
  auto header_h = fs::File(dir_path + "/include/header.h", "#pragma once");
  auto test_cpp = fs::File(dir_path + "/tests/test.cpp", "#include <gtest/gtest.h>");

  src_dir.add_file(std::move(main_cpp));
  include_dir.add_file(std::move(header_h));
  tests_dir.add_file(std::move(test_cpp));

  root.add_subdir(std::move(src_dir));
  root.add_subdir(std::move(include_dir));
  root.add_subdir(std::move(tests_dir));

  root.write_to_disk();

  EXPECT_TRUE(file_exists(dir_path + "/src/main.cpp"));
  EXPECT_TRUE(file_exists(dir_path + "/include/header.h"));
  EXPECT_TRUE(file_exists(dir_path + "/tests/test.cpp"));
}

} // namespace fs_test
