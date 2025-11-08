#include "../include/presets.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace presets_test {

// Test fixture for presets tests
class PresetsTest : public ::testing::Test {
protected:
  std::string test_dir = "./test_presets_output";
  std::string test_preset_file = test_dir + "/test_presets.cdndb";
  std::string test_lua_file = test_dir + "/test_script.lua";

  void SetUp() override {
    // Clean up test directory before each test
    std::filesystem::remove_all(test_dir);
    std::filesystem::create_directories(test_dir);

    // Create a test Lua script
    std::ofstream lua_file(test_lua_file);
    lua_file << "-- Test Lua script\nprint('Hello from preset')";
    lua_file.close();
  }

  void TearDown() override {
    // Clean up test directory after each test
    std::filesystem::remove_all(test_dir);
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
// Preset Tests
// ============================================================================

TEST_F(PresetsTest, PresetConstruction) {
  Presets::Preset preset1;

  Presets::Preset preset2("test_preset", "/path/to/script.lua");
  EXPECT_EQ(preset2.get_name(), "test_preset");
  EXPECT_EQ(preset2.get_path(), "/path/to/script.lua");
}

TEST_F(PresetsTest, PresetGetters) {
  Presets::Preset preset("my_preset", "/my/path.lua");

  EXPECT_EQ(preset.get_name(), "my_preset");
  EXPECT_EQ(preset.get_path(), "/my/path.lua");
}

TEST_F(PresetsTest, PresetPrint) {
  Presets::Preset preset("test", "/test/path.lua");

  // Just ensure print doesn't crash
  testing::internal::CaptureStdout();
  preset.print();
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_TRUE(output.find("test") != std::string::npos);
  EXPECT_TRUE(output.find("/test/path.lua") != std::string::npos);
}

// ============================================================================
// PresetManager Tests
// ============================================================================

TEST_F(PresetsTest, PresetManagerConstruction) {
  Presets::PresetManager manager;
  EXPECT_TRUE(manager.list_presets().empty());
}

TEST_F(PresetsTest, AddPreset) {
  Presets::PresetManager manager;
  Presets::Preset preset("preset1", "/path1.lua");

  manager.add_preset(preset);

  auto presets = manager.list_presets();
  EXPECT_EQ(presets.size(), 1);
  EXPECT_EQ(presets[0].get_name(), "preset1");
  EXPECT_EQ(presets[0].get_path(), "/path1.lua");
}

TEST_F(PresetsTest, AddMultiplePresets) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("preset1", "/path1.lua"));
  manager.add_preset(Presets::Preset("preset2", "/path2.lua"));
  manager.add_preset(Presets::Preset("preset3", "/path3.lua"));

  auto presets = manager.list_presets();
  EXPECT_EQ(presets.size(), 3);
}

TEST_F(PresetsTest, RemovePreset) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("preset1", "/path1.lua"));
  manager.add_preset(Presets::Preset("preset2", "/path2.lua"));
  manager.add_preset(Presets::Preset("preset3", "/path3.lua"));

  manager.remove_preset("preset2");

  auto presets = manager.list_presets();
  EXPECT_EQ(presets.size(), 2);
  EXPECT_EQ(presets[0].get_name(), "preset1");
  EXPECT_EQ(presets[1].get_name(), "preset3");
}

TEST_F(PresetsTest, RemoveNonExistentPreset) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("preset1", "/path1.lua"));
  manager.remove_preset("nonexistent");

  auto presets = manager.list_presets();
  EXPECT_EQ(presets.size(), 1);
}

TEST_F(PresetsTest, GetPreset) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("preset1", "/path1.lua"));
  manager.add_preset(Presets::Preset("preset2", "/path2.lua"));

  auto *preset = manager.get_preset("preset1");
  ASSERT_NE(preset, nullptr);
  EXPECT_EQ(preset->get_name(), "preset1");
  EXPECT_EQ(preset->get_path(), "/path1.lua");
}

TEST_F(PresetsTest, GetNonExistentPreset) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("preset1", "/path1.lua"));

  auto *preset = manager.get_preset("nonexistent");
  EXPECT_EQ(preset, nullptr);
}

TEST_F(PresetsTest, SavePresetsToFile) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("preset1", "/path1.lua"));
  manager.add_preset(Presets::Preset("preset2", "/path2.lua"));

  manager.save_presets_to_file(test_preset_file);

  EXPECT_TRUE(std::filesystem::exists(test_preset_file));

  std::string content = read_file(test_preset_file);
  EXPECT_TRUE(content.find("preset1") != std::string::npos);
  EXPECT_TRUE(content.find("/path1.lua") != std::string::npos);
  EXPECT_TRUE(content.find("preset2") != std::string::npos);
  EXPECT_TRUE(content.find("/path2.lua") != std::string::npos);
}

TEST_F(PresetsTest, SavePresetsFileFormat) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("test", "/test.lua"));
  manager.save_presets_to_file(test_preset_file);

  std::string content = read_file(test_preset_file);
  // Check the CSV-like format: "name","path"
  EXPECT_TRUE(content.find("\"test\",\"/test.lua\"") != std::string::npos);
}

TEST_F(PresetsTest, LoadPresetsFromFile) {
  // Create a preset file manually
  std::ofstream file(test_preset_file);
  file << "\"preset1\",\"/path1.lua\"\n";
  file << "\"preset2\",\"/path2.lua\"\n";
  file.close();

  auto manager =
      Presets::PresetManager::load_presets_from_file(test_preset_file);

  auto presets = manager.list_presets();
  EXPECT_EQ(presets.size(), 2);
  EXPECT_EQ(presets[0].get_name(), "preset1");
  EXPECT_EQ(presets[0].get_path(), "/path1.lua");
  EXPECT_EQ(presets[1].get_name(), "preset2");
  EXPECT_EQ(presets[1].get_path(), "/path2.lua");
}

TEST_F(PresetsTest, LoadPresetsFromEmptyFile) {
  // Create an empty preset file
  std::ofstream file(test_preset_file);
  file.close();

  auto manager =
      Presets::PresetManager::load_presets_from_file(test_preset_file);

  auto presets = manager.list_presets();
  EXPECT_TRUE(presets.empty());
}

TEST_F(PresetsTest, LoadPresetsFromNonExistentFile) {
  // Should throw an exception
  EXPECT_THROW(
      Presets::PresetManager::load_presets_from_file("/nonexistent/file.cdndb"),
      std::runtime_error);
}

TEST_F(PresetsTest, SaveAndLoadRoundTrip) {
  Presets::PresetManager manager1;

  manager1.add_preset(Presets::Preset("preset1", "/path1.lua"));
  manager1.add_preset(Presets::Preset("preset2", "/path2.lua"));
  manager1.add_preset(Presets::Preset("preset3", "/path3.lua"));

  manager1.save_presets_to_file(test_preset_file);

  auto manager2 =
      Presets::PresetManager::load_presets_from_file(test_preset_file);

  auto presets1 = manager1.list_presets();
  auto presets2 = manager2.list_presets();

  EXPECT_EQ(presets1.size(), presets2.size());
  for (size_t i = 0; i < presets1.size(); ++i) {
    EXPECT_EQ(presets1[i].get_name(), presets2[i].get_name());
    EXPECT_EQ(presets1[i].get_path(), presets2[i].get_path());
  }
}

TEST_F(PresetsTest, ListPresetsEmpty) {
  Presets::PresetManager manager;
  auto presets = manager.list_presets();
  EXPECT_TRUE(presets.empty());
}

TEST_F(PresetsTest, ListPresets) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("a", "/a.lua"));
  manager.add_preset(Presets::Preset("b", "/b.lua"));

  auto presets = manager.list_presets();
  EXPECT_EQ(presets.size(), 2);
}

TEST_F(PresetsTest, PresetWithSpecialCharactersInName) {
  Presets::PresetManager manager;

  manager.add_preset(Presets::Preset("my-preset_v2.0", "/path.lua"));
  manager.save_presets_to_file(test_preset_file);

  auto manager2 =
      Presets::PresetManager::load_presets_from_file(test_preset_file);
  auto presets = manager2.list_presets();

  EXPECT_EQ(presets.size(), 1);
  EXPECT_EQ(presets[0].get_name(), "my-preset_v2.0");
}

} // namespace presets_test
