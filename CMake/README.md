# CMake support for BWAPI-Client

This is CMake project which can be included in your bot application. 
**NOTE** You get two static libraries. 
They are not usable for DLL bots.

These two libraries can only be linked with your client-mode bot (see
`bwapi/ExampleAIClient` for an example).

## Why?

This project does not require VS2013, and can be built with other compilers,
such as VS2015, VS2017 and, potentially, GCC with C++17 enabled.

## How to use?

In your `CMakeLists.txt` add a line **before** your `PROJECT(...)`:

    ADD_SUBDIRECTORY(${MY_PATH_TO_BWAPI}/CMake/BWAPI/)
    ADD_SUBDIRECTORY(${MY_PATH_TO_BWAPI}/CMake/Client/)

In your `CMakeLists.txt` add lines **inside** your `PROJECT(...)`:

    GET_FILENAME_COMPONENT(BWAPI_ROOT ${CMAKE_SOURCE_DIR}/deps/bwapi/bwapi ABSOLUTE)
    INCLUDE_DIRECTORIES(${BWAPI_ROOT}/include ${BWAPI_ROOT}/Util/Source)

And in linker section add lines:

    ADD_DEPENDENCIES(YourProject BWAPI-Staticd)
    ADD_DEPENDENCIES(YourProject BWAPIClientd)

If you build as release, remove `d` and use `BWAPI` and `BWAPIClient` instead.

## Custom compile flags

It may happen so, that your project uses different compile flags than those,
selected by default in these BWAPI projects. In such case set the 
variable `BWAPI_CUSTOM_COMPILE_FLAGS` to contain compile flags before you
include CMake subdirectories.
