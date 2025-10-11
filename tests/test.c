#include "../include/dir.h"
#include "../include/init.h"
#include "../include/log.h"
#include "../include/path.h"
#include "../include/preset.h"

int preset_tests() {
  Preset fromStringHappy = fromString("\"test\",\"/path/to/test\"");
  if (strcmp(fromStringHappy.name, "test") != 0) {
    log_error("fromString happy name test failed, got name: %s",
              fromStringHappy.name);
    return 1;
  }
  if (strcmp(fromStringHappy.path, "/path/to/test") != 0) {
    log_error("fromString happy path test failed, got path: %s",
              fromStringHappy.path);
    return 1;
  }

  Preset fromStringEmpty = fromString("");
  if (fromStringEmpty.name != NULL) {
    log_error("fromString empty name test failed, got name: %s",
              fromStringEmpty.name);
    return 1;
  }
  if (fromStringEmpty.path != NULL) {
    log_error("fromString empty path test failed, got path: %s",
              fromStringEmpty.path);
    return 1;
  }
}

int tests() {
  if (preset_tests() != 0) {
    return 1;
  }

  return 0;
}
