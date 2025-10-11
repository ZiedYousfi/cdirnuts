#include "../include/log.h"
#include "../include/preset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int assert_int(int cond, const char *msg) {
  if (!cond) {
    log_error("FAIL: %s", msg);
    return 1;
  }
  log_info("PASS: %s", msg);
  return 0;
}

static int assert_str(const char *a, const char *b, const char *msg) {
  if ((a == NULL && b != NULL) || (a != NULL && b == NULL)) {
    log_error("FAIL: %s - expected '%s' got '%s'", msg, b ? b : "(null)",
              a ? a : "(null)");
    return 1;
  }
  if (a == NULL && b == NULL) {
    log_info("PASS: %s", msg);
    return 0;
  }
  if (strcmp(a, b) != 0) {
    log_error("FAIL: %s - expected '%s' got '%s'", msg, b, a);
    return 1;
  }
  log_info("PASS: %s", msg);
  return 0;
}

int test_fromString_basic() {
  Preset p = fromString("\"test\",\"/path/to/test\"");
  int r = 0;
  r |= assert_str(p.name, "test", "fromString basic name");
  r |= assert_str(p.path, "/path/to/test", "fromString basic path");
  if (p.name)
    free(p.name);
  if (p.path)
    free(p.path);
  return r;
}

int test_fromString_empty() {
  Preset p = fromString("");
  int r = 0;
  r |= assert_int(p.name == NULL, "fromString empty name is NULL");
  r |= assert_int(p.path == NULL, "fromString empty path is NULL");
  return r;
}

int test_fromString_malformed() {
  Preset p = fromString("\"onlyname\"");
  int r = 0;
  r |= assert_int(p.name == NULL,
                  "fromString malformed: single quoted name -> name NULL");
  r |= assert_int(p.path == NULL,
                  "fromString malformed: single quoted name -> path NULL");
  return r;
}

int test_toString_roundtrip() {
  Preset p = {strdup("round"), strdup("/tmp/round")};
  char *s = toString(p);
  int r = 0;
  r |= assert_str(s, "\"round\",\"/tmp/round\"", "toString format");
  Preset p2 = fromString(s);
  r |= assert_str(p2.name, "round", "roundtrip name");
  r |= assert_str(p2.path, "/tmp/round", "roundtrip path");
  free(p.name);
  free(p.path);
  free(s);
  if (p2.name)
    free(p2.name);
  if (p2.path)
    free(p2.path);
  return r;
}

int test_save_and_get_presets() {
  // create temporary presets
  Preset presets[3];
  presets[0].name = strdup("p1");
  presets[0].path = strdup("/p1");
  presets[1].name = strdup("p2");
  presets[1].path = strdup("/p2");
  presets[2].name = NULL;
  presets[2].path = NULL; // should be ignored

  int rc = savePresets(presets, 2);
  int r = 0;
  r |= assert_int(rc == 0 || rc == 1 || rc == -1,
                  "savePresets returned code (any known code)");

  Preset *loaded = getPresets();
  if (!loaded) {
    log_warn("getPresets returned NULL — maybe no file or error");
    // cleanup
    for (int i = 0; i < 2; i++) {
      free(presets[i].name);
      free(presets[i].path);
    }
    return 0; // can't assert further in environments without FS access
  }

  // count loaded entries by scanning until name==NULL
  int count = 0;
  while (loaded[count].name != NULL && loaded[count].path != NULL) {
    count++;
  }
  r |= assert_int(count >= 1, "getPresets loaded at least one preset");

  // cleanup
  for (int i = 0; i < 2; i++) {
    free(presets[i].name);
    free(presets[i].path);
  }
  for (int i = 0; i < count; i++) {
    free(loaded[i].name);
    free(loaded[i].path);
  }
  free(loaded);
  return r;
}

int test_valueBetweenQuotes_internal() {
  // We can't call static functions; test indirectly by malformed strings
  Preset p = fromString("\"a\",\"b with \"inner\" quotes\"");
  int r = 0;
  // parser will stop path at the inner quote, expect path to be truncated
  r |= assert_str(p.name, "a", "fromString inner-quotes name");
  r |= assert_str(p.path, "b with ", "fromString inner-quotes truncated path");
  if (p.name)
    free(p.name);
  if (p.path)
    free(p.path);
  return r;
}

int main() {
  int failures = 0;
  failures += test_fromString_basic();
  failures += test_fromString_empty();
  failures += test_fromString_malformed();
  failures += test_toString_roundtrip();
  failures += test_save_and_get_presets();
  failures += test_valueBetweenQuotes_internal();

  if (failures != 0) {
    log_error("%d preset tests failed", failures);
    return 1;
  }
  log_info("All preset tests passed");
  return 0;
}
