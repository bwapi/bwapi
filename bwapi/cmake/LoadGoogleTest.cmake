include(FetchContent)

set(GTEST_VERSION 1.10.0)

# Set Googletest CMake options
option(BUILD_GMOCK "" ON)
option(INSTALL_GTEST "" OFF)
option(BUILD_SHARED_LIBS "" OFF)

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
