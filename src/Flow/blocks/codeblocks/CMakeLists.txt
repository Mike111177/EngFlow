find_package(Lua)

add_library(LuaBlock LuaBlock.cpp)
target_include_directories(LuaBlock PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(LuaBlock ${LUA_LIBRARY})