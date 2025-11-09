#include "../include/lua.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace lua_test {

// Test fixture for Lua tests
class LuaTest : public ::testing::Test {
protected:
  std::string test_dir = "./test_lua_output";
  std::string test_lua_file = test_dir + "/test_script.lua";

  void SetUp() override {
    // Clean up test directory before each test
    std::filesystem::remove_all(test_dir);
    std::filesystem::create_directories(test_dir);
  }

  void TearDown() override {
    // Clean up test directory after each test
    std::filesystem::remove_all(test_dir);
  }

  void create_lua_file(const std::string &content) {
    std::ofstream file(test_lua_file);
    file << content;
    file.close();
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
// LuaEngine Basic Tests
// ============================================================================

TEST_F(LuaTest, LuaEngineConstruction) {
  // Should not throw
  EXPECT_NO_THROW({ Lua::LuaEngine lua; });
}

TEST_F(LuaTest, ExecuteSimpleString) {
  Lua::LuaEngine lua;

  // Execute simple Lua code
  EXPECT_NO_THROW({ lua.execute_string("x = 5 + 3"); });
}

TEST_F(LuaTest, ExecuteFile) {
  create_lua_file("-- Simple test script\nx = 10");

  Lua::LuaEngine lua;
  EXPECT_NO_THROW({ lua.execute_file(test_lua_file); });
}

TEST_F(LuaTest, ExecuteNonExistentFile) {
  Lua::LuaEngine lua;

  // Should throw an exception
  EXPECT_THROW({ lua.execute_file("/nonexistent/file.lua"); }, std::exception);
}

// ============================================================================
// LuaEngine API Tests
// ============================================================================

TEST_F(LuaTest, ApiGetCWD) {
  Lua::LuaEngine lua;

  std::string script = R"(
    local cwd = cdirnuts.getCWD()
    assert(cwd ~= nil, "getCWD should return a value")
    assert(type(cwd) == "string", "getCWD should return a string")
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });
}

TEST_F(LuaTest, ApiCreateVirtualDir) {
  Lua::LuaEngine lua;

  std::string script = R"(
    local dir = cdirnuts.create_virtual_dir("test_dir")
    assert(dir ~= nil, "create_virtual_dir should return a value")
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });
}

TEST_F(LuaTest, ApiCreateVirtualFile) {
  Lua::LuaEngine lua;

  std::string script = R"(
    local file = cdirnuts.create_virtual_file("test.txt", "content")
    assert(file ~= nil, "create_virtual_file should return a value")
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });
}

TEST_F(LuaTest, ApiWriteVirtualFile) {
  Lua::LuaEngine lua;

  std::string script = R"(
    local file = cdirnuts.create_virtual_file(")" +
                       test_dir + R"(/test.txt", "Hello World")
    cdirnuts.write_virtual_file(file)
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });

  EXPECT_TRUE(file_exists(test_dir + "/test.txt"));
  EXPECT_EQ(read_file(test_dir + "/test.txt"), "Hello World");
}

TEST_F(LuaTest, ApiWriteVirtualDir) {
  Lua::LuaEngine lua;

  std::string script = R"(
    local dir = cdirnuts.create_virtual_dir(")" +
                       test_dir + R"(/new_dir")
    cdirnuts.write_virtual_dir(dir)
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });

  EXPECT_TRUE(std::filesystem::exists(test_dir + "/new_dir"));
  EXPECT_TRUE(std::filesystem::is_directory(test_dir + "/new_dir"));
}

TEST_F(LuaTest, ApiAppendFile) {
  Lua::LuaEngine lua;

  std::string script = R"(
    local dir = cdirnuts.create_virtual_dir(")" +
                       test_dir + R"(/parent")
    local file = cdirnuts.create_virtual_file(")" +
                       test_dir + R"(/parent/child.txt", "content")
    cdirnuts.append_file(dir, file)
    cdirnuts.write_virtual_dir(dir)
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });

  EXPECT_TRUE(file_exists(test_dir + "/parent/child.txt"));
  EXPECT_EQ(read_file(test_dir + "/parent/child.txt"), "content");
}

TEST_F(LuaTest, ApiAppendSubdir) {
  Lua::LuaEngine lua;

  std::string script = R"(
    local parent = cdirnuts.create_virtual_dir(")" +
                       test_dir + R"(/parent")
    local child = cdirnuts.create_virtual_dir(")" +
                       test_dir + R"(/parent/child")
    cdirnuts.append_subdir(parent, child)
    cdirnuts.write_virtual_dir(parent)
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });

  EXPECT_TRUE(std::filesystem::exists(test_dir + "/parent/child"));
  EXPECT_TRUE(std::filesystem::is_directory(test_dir + "/parent/child"));
}

TEST_F(LuaTest, ApiComplexStructure) {
  Lua::LuaEngine lua;

  std::string script = R"(
    -- Create a project structure
    local root = cdirnuts.create_virtual_dir(")" +
                       test_dir + R"(/project")
    local src = cdirnuts.create_virtual_dir(")" +
                       test_dir + R"(/project/src")
    local include = cdirnuts.create_virtual_dir(")" +
                       test_dir + R"(/project/include")

    local main_cpp = cdirnuts.create_virtual_file(")" +
                       test_dir + R"(/project/src/main.cpp", "int main() {}")
    local header_h = cdirnuts.create_virtual_file(")" +
                       test_dir + R"(/project/include/header.h", "#pragma once")

    cdirnuts.append_file(src, main_cpp)
    cdirnuts.append_file(include, header_h)
    cdirnuts.append_subdir(root, src)
    cdirnuts.append_subdir(root, include)

    cdirnuts.write_virtual_dir(root)
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });

  EXPECT_TRUE(file_exists(test_dir + "/project/src/main.cpp"));
  EXPECT_TRUE(file_exists(test_dir + "/project/include/header.h"));
  EXPECT_EQ(read_file(test_dir + "/project/src/main.cpp"), "int main() {}");
  EXPECT_EQ(read_file(test_dir + "/project/include/header.h"), "#pragma once");
}

TEST_F(LuaTest, ApiMultipleFiles) {
  Lua::LuaEngine lua;

  std::string script = R"(
    local dir = cdirnuts.create_virtual_dir(")" +
                       test_dir + R"(/multi")

    for i = 1, 5 do
      local file = cdirnuts.create_virtual_file(")" +
                       test_dir +
                       R"(/multi/file" .. i .. ".txt", "Content " .. i)
      cdirnuts.append_file(dir, file)
    end

    cdirnuts.write_virtual_dir(dir)
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });

  for (int i = 1; i <= 5; ++i) {
    std::string filename =
        test_dir + "/multi/file" + std::to_string(i) + ".txt";
    EXPECT_TRUE(file_exists(filename));
    EXPECT_EQ(read_file(filename), "Content " + std::to_string(i));
  }
}

TEST_F(LuaTest, LuaStandardLibraries) {
  Lua::LuaEngine lua;

  // Test that standard libraries are loaded
  std::string script = R"(
    -- Test string library
    local s = string.upper("hello")
    assert(s == "HELLO", "String library not working")

    -- Test io library
    assert(io ~= nil, "IO library not loaded")

    -- Test base library
    assert(print ~= nil, "Base library not loaded")
  )";

  EXPECT_NO_THROW({ lua.execute_string(script); });
}

TEST_F(LuaTest, LuaErrorHandling) {
  Lua::LuaEngine lua;

  // Invalid Lua code should throw
  EXPECT_THROW(
      { lua.execute_string("this is invalid lua code @#$"); }, std::exception);
}

TEST_F(LuaTest, LuaFileWithSyntaxError) {
  create_lua_file("this is @#$ invalid lua");

  Lua::LuaEngine lua;

  EXPECT_THROW({ lua.execute_file(test_lua_file); }, std::exception);
}

TEST_F(LuaTest, MultipleExecutions) {
  Lua::LuaEngine lua;

  // State should persist across executions
  EXPECT_NO_THROW({
    lua.execute_string("x = 10");
    lua.execute_string("y = x + 5");
    lua.execute_string("assert(y == 15, 'State not persisted')");
  });
}

} // namespace lua_test
