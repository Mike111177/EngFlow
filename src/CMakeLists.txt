add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Flow)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sdl)

add_executable(EngFlow main.cpp)
target_link_libraries(EngFlow Flow FlowUI SDL_WINDOW)
target_include_directories(EngFlow PRIVATE ${PROJECT_SOURCE_DIR}/include )