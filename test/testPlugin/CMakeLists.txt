include(GenerateExportHeader)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
add_library(FlowPluginTest MODULE testPlugin.cpp)
generate_export_header(FlowPluginTest)
target_include_directories(FlowPluginTest PRIVATE ${CMAKE_CURRENT_BINARY_DIR})