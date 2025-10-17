#include <gtest/gtest.h>

#include "../../src/config/Config.hpp"

#include <filesystem>

namespace fs = std::filesystem;

class ConfigTestFixture : public ::testing::Test {
protected:
  void SetUp() override {
    test_configs_path = fs::path(__FILE__).parent_path() / "test_configs";

    auto& config = ayan::cfg::Config::Instance();
  }

  void TearDown() override {}

  std::string GetTestConfigPath(const std::string& filename) {
    return (test_configs_path / filename).string();
  }

private:
  fs::path test_configs_path;
};

TEST_F(ConfigTestFixture, SingletonInstance) {
  auto& config1 = ayan::cfg::Config::Instance();
  auto& config2 = ayan::cfg::Config::Instance();

  EXPECT_EQ(&config1, &config2);
}

TEST_F(ConfigTestFixture, LoadValidBasicConfig) {
  auto& config = ayan::cfg::Config::Instance();
  auto result = config.load_params_from_file(GetTestConfigPath("valid_basic.ini"));

  EXPECT_TRUE(result.is_ok());
  EXPECT_TRUE(result.unwrap_value());
}

TEST_F(ConfigTestFixture, LoadNonExistentFile) {
  auto& config = ayan::cfg::Config::Instance();
  auto result = config.load_params_from_file("non_existent_file.ini");

  EXPECT_TRUE(result.is_err());
  auto error = result.unwrap_err();
  EXPECT_NE(std::string(error.what()).find("Cannot open"), std::string::npos);
}

TEST_F(ConfigTestFixture, LoadEmptyFile) {
  auto& config = ayan::cfg::Config::Instance();
  auto result = config.load_params_from_file(GetTestConfigPath("empty_file.ini"));

  EXPECT_TRUE(result.is_ok());
}

TEST_F(ConfigTestFixture, LoadConfigWithComments) {
  auto& config = ayan::cfg::Config::Instance();
  auto result = config.load_params_from_file(GetTestConfigPath("with_comments.ini"));

  EXPECT_TRUE(result.is_ok());

  auto app_name = config.get_param_as_string("app_name");
  EXPECT_TRUE(app_name.is_ok());
  EXPECT_EQ(app_name.unwrap_value(), "CommentedApp");

  auto max_users = config.get_param_as_int("max_users");
  EXPECT_TRUE(max_users.is_ok());
  EXPECT_EQ(max_users.unwrap_value(), 50);
}

TEST_F(ConfigTestFixture, LoadInvalidFormatConfig) {
  auto& config = ayan::cfg::Config::Instance();
  auto result = config.load_params_from_file(GetTestConfigPath("invalid_format.ini"));

  EXPECT_TRUE(result.is_err());
}

TEST_F(ConfigTestFixture, GetStringParameter) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("valid_basic.ini"));

  auto app_name = config.get_param_as_string("app_name");
  EXPECT_TRUE(app_name.is_ok());
  EXPECT_EQ(app_name.unwrap_value(), "MyTestApp");

  auto log_level = config.get_param_as_string("log_level");
  EXPECT_TRUE(log_level.is_ok());
  EXPECT_EQ(log_level.unwrap_value(), "DEBUG");
}

TEST_F(ConfigTestFixture, GetNonExistentStringParameter) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("valid_basic.ini"));

  auto result = config.get_param_as_string("non_existent_param");
  EXPECT_TRUE(result.is_err());
}

TEST_F(ConfigTestFixture, GetIntParameter) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("valid_basic.ini"));

  auto max_connections = config.get_param_as_int("max_connections");
  EXPECT_TRUE(max_connections.is_ok());
  EXPECT_EQ(max_connections.unwrap_value(), 100);
}

TEST_F(ConfigTestFixture, GetVariousIntFormats) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("types_test.ini"));

  auto positive = config.get_param_as_int("positive_int");
  EXPECT_TRUE(positive.is_ok());
  EXPECT_EQ(positive.unwrap_value(), 42);

  auto negative = config.get_param_as_int("negative_int");
  EXPECT_TRUE(negative.is_ok());
  EXPECT_EQ(negative.unwrap_value(), -100);

  auto zero = config.get_param_as_int("zero");
  EXPECT_TRUE(zero.is_ok());
  EXPECT_EQ(zero.unwrap_value(), 0);

  auto large = config.get_param_as_int("large_int");
  EXPECT_TRUE(large.is_ok());
  EXPECT_EQ(large.unwrap_value(), 999999);
}

TEST_F(ConfigTestFixture, GetDoubleParameter) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("valid_basic.ini"));

  auto timeout = config.get_param_as_double("timeout");
  EXPECT_TRUE(timeout.is_ok());
  EXPECT_DOUBLE_EQ(timeout.unwrap_value(), 30.5);
}

TEST_F(ConfigTestFixture, GetVariousDoubleFormats) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("types_test.ini"));

  auto positive = config.get_param_as_double("float_positive");
  EXPECT_TRUE(positive.is_ok());
  EXPECT_DOUBLE_EQ(positive.unwrap_value(), 3.14);

  auto negative = config.get_param_as_double("float_negative");
  EXPECT_TRUE(negative.is_ok());
  EXPECT_DOUBLE_EQ(negative.unwrap_value(), -2.71);

  auto scientific = config.get_param_as_double("scientific");
  EXPECT_TRUE(scientific.is_ok());
  EXPECT_DOUBLE_EQ(scientific.unwrap_value(), 1.23e-4);
}

TEST_F(ConfigTestFixture, GetBoolParameter) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("valid_basic.ini"));

  auto enable_feature = config.get_param_as_bool("enable_feature");
  EXPECT_TRUE(enable_feature.is_ok());
  EXPECT_TRUE(enable_feature.unwrap_value());
}

TEST_F(ConfigTestFixture, GetVariousBoolFormats) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("types_test.ini"));

  EXPECT_TRUE(config.get_param_as_bool("bool_true1").unwrap_value());
  EXPECT_TRUE(config.get_param_as_bool("bool_true2").unwrap_value());
  EXPECT_TRUE(config.get_param_as_bool("bool_true3").unwrap_value());
  EXPECT_TRUE(config.get_param_as_bool("bool_true4").unwrap_value());

  EXPECT_FALSE(config.get_param_as_bool("bool_false1").unwrap_value());
  EXPECT_FALSE(config.get_param_as_bool("bool_false2").unwrap_value());
  EXPECT_FALSE(config.get_param_as_bool("bool_false3").unwrap_value());
  EXPECT_FALSE(config.get_param_as_bool("bool_false4").unwrap_value());
}

TEST_F(ConfigTestFixture, GetInvalidBoolParameter) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("edge_cases.ini"));

  auto result = config.get_param_as_bool("numeric_string");
  EXPECT_TRUE(result.is_err());
}

TEST_F(ConfigTestFixture, EdgeCases) {
  auto& config = ayan::cfg::Config::Instance();
  config.load_params_from_file(GetTestConfigPath("edge_cases.ini"));

  auto special = config.get_param_as_string("special_chars");
  EXPECT_TRUE(special.is_ok());
  EXPECT_EQ(special.unwrap_value(), "value_with_underscore-and-dash");

  auto spaces = config.get_param_as_string("spaces_in_value");
  EXPECT_TRUE(spaces.is_ok());
  EXPECT_EQ(spaces.unwrap_value(), "valuewithspaces");

  auto empty = config.get_param_as_string("empty_value");
  EXPECT_TRUE(empty.is_ok());
  EXPECT_EQ(empty.unwrap_value(), "");
}
