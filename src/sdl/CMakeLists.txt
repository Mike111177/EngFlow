find_package(SDL2 REQUIRED)

add_Library(SDL_WINDOW Window.cpp)
target_include_directories(SDL_WINDOW PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(SDL_WINDOW FlowUI ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY})