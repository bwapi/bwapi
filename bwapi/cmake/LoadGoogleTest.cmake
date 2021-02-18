include(FetchContent)

set(GTEST_VERSION 1.10.0)

# Set Googletest CMake options
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Get Googletest source code
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-${GTEST_VERSION}
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

FetchContent_MakeAvailable(googletest)

if(MSVC)
  set_target_properties(gmock gmock_main gtest gtest_main PROPERTIES FOLDER "Third Party/googletest")
endif()
