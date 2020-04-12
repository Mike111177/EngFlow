enable_testing()

include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    doctest
    PREFIX ${CMAKE_BINARY_DIR}/doctest
    GIT_REPOSITORY https://github.com/onqtam/doctest.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(doctest source_dir)
set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")

set(TEST_SOURCES testMain.cpp testFlowDocument.cpp)
set(TEST_LIBS Flow)

#Determine Additional Availible Tests
if (FLOW_BUILD_LUABLOCK) #LuaBlock
    list(APPEND TEST_SOURCES testLuaBlock.cpp)
        list(APPEND TEST_LIBS LuaBlock)
endif()

#Determine Additional Availible Tests
if (FLOW_BUILD_PYTHONBLOCK) #LuaBlock
    list(APPEND TEST_SOURCES testPythonBlock.cpp)
    list(APPEND TEST_LIBS PythonBlock)
endif()

add_executable(FlowTest ${TEST_SOURCES})
target_link_libraries(FlowTest ${TEST_LIBS})
#Add doctest
target_include_directories(FlowTest PUBLIC ${DOCTEST_INCLUDE_DIR})
add_dependencies(FlowTest doctest)
if (FLOW_RUN_TESTS)
    add_custom_command(TARGET FlowTest
                       POST_BUILD
                       COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> -R FlowTest --output-on-failure)
endif()
add_test(NAME RunFlowTest COMMAND FlowTest)




