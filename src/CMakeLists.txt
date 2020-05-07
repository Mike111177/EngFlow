add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Util)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Flow)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sdl)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/opengl)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ui)

add_executable(EngFlow main.cpp)
target_link_libraries(EngFlow Flow FlowUI SDL_WINDOW FlowGL)