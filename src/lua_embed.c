#include "../include/lua_embed.h"

static void run_lua_smoke_test(void) {
  lua_State *L = luaL_newstate();
  if (!L) {
    fprintf(stderr, "Failed to create Lua state\n");
    return;
  }
  luaL_openlibs(L);

  const char *script =
      "print('Lua OK: 2+3=', 2+3)\n"
      "function add(a,b) return a+b end";

  if (luaL_dostring(L, script) != LUA_OK) {
    fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
    lua_close(L);
    return;
  }

  lua_getglobal(L, "add");
  lua_pushnumber(L, 10);
  lua_pushnumber(L, 32);
  if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
    fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  } else {
    double res = lua_tonumber(L, -1);
    printf("add(10,32) = %.0f\n", res);
    lua_pop(L, 1);
  }

  lua_close(L);
}
