#include <gtest/gtest.h>

#include <filesystem>

#include "base_test.hpp"
#include "hyrise/utils/assert.hpp"
#include "hyrise/utils/performance_warning.hpp"

std::string hyrise::test_data_path;        // NOLINT
std::string hyrise::test_executable_path;  // NOLINT

void create_test_data_directory(std::optional<std::string>& prefix) {
  Assert(!std::filesystem::exists(hyrise::test_data_path),
         "Cannot create directory for test data: \"" + hyrise::test_data_path + "\" already exists.");

  if (prefix) {
    Assert(std::filesystem::exists("./" + *prefix),
           "Cannot create directory for test data because \"" + *prefix + "\" does not exist");
  }

  std::filesystem::create_directory(hyrise::test_data_path);
}

void remove_test_data_directory() {
  if (std::filesystem::exists(hyrise::test_data_path)) {
    std::filesystem::remove_all(hyrise::test_data_path);
  }
}

int main(int argc, char** argv) {
  Assert(std::filesystem::exists("../Src/hyrise/resources/test_data/tbl"),
         "Cannot find resources/test_data/tbl. Are you running the test suite from the main folder of the Hyrise "
         "repository?");

  auto performance_warning_disabler = hyrise::PerformanceWarningDisabler{};
  ::testing::InitGoogleTest(&argc, argv);

  auto prefix = std::optional<std::string>{};
  if (argc > 1) {
    // If argv[1] is set after gtest extracted its commands, we interpret it as directory name prefix for test data.
    hyrise::test_data_path = "./" + std::string(argv[1]) + "/.hyrise_test_data/";
    prefix = argv[1];
  } else {
    hyrise::test_data_path = "./.hyrise_test_data/";
  }
  hyrise::test_executable_path = std::filesystem::canonical("./" + std::string{argv[0]}).remove_filename();
  remove_test_data_directory();
  create_test_data_directory(prefix);

  int ret = RUN_ALL_TESTS();

  remove_test_data_directory();

  return ret;
}