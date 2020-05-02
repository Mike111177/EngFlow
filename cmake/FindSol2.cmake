find_path(SOL2_INCLUDE_DIRS
    NAMES sol/sol.hpp
    HINTS "${PROJECT_SOURCE_DIR}/3rdparty/sol2/include"
    DOC "The sol2 include directory"
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Sol2 DEFAULT_MSG SOL2_INCLUDE_DIRS)