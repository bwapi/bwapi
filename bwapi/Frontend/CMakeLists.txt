cmake_minimum_required(VERSION 3.16)

#
# The BWAPILIB Coverage target is in here because gcov/lcov cannot include sources outside of its directory scope
#
# Usage: make 
if(CMAKE_COMPILER_IS_GNUCXX AND (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL ""))
  include(CodeCoverage)

  setup_target_for_coverage_lcov(
    NAME BWAPILIB_coverage
    EXECUTABLE BWAPILIBUnitTest --gtest_shuffle
    DEPENDENCIES BWAPILIBUnitTest BWAPILIB
    BASE_DIRECTORY ${BWAPILIB_SOURCE_DIR}
    EXCLUDE "Frontend/BWAPILIBUnitTest/*" "/usr/*" "build/*"
    LCOV_ARGS --rc lcov_branch_coverage=1
    GENHTML_ARGS --rc genhtml_branch_coverage=1
  )

  setup_target_for_coverage_lcov(
    NAME FunctionalTests_coverage
    EXECUTABLE FunctionalTests --gtest_shuffle
    DEPENDENCIES FunctionalTests BWAPILIB
    BASE_DIRECTORY ${BWAPILIB_SOURCE_DIR}
    EXCLUDE "Frontend/FunctionalTests/*" "/usr/*" "build/*"
    LCOV_ARGS --rc lcov_branch_coverage=1
    GENHTML_ARGS --rc genhtml_branch_coverage=1
  )

endif()
