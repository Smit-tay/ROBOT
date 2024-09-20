cmake_minimum_required (VERSION 3.22.0)
set (CMAKE_VERBOSE_MAKEFILE FALSE)

project (Steampunk_Clock
         VERSION 0.1
         DESCRIPTION "uArm SwiftPro project to make a digital clock in the Steampunk style"
         HOMEPAGE_URL http:/blah/blah.html
         LANGUAGES C CXX)


if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
  endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This needs to be looked at - is linux=1 necessary ?
add_compile_options("-D__linux__=1"
                    "-D_REENTRANT"
                    "-Wall" "-Wextra"
                    "-fno-strict-aliasing")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(SOURCES
    src/main.cpp
    src/SwiftController.cpp
    src/SwiftPositions.cpp
    src/TimeUpdater.cpp
#    src/UIManager.cpp
)
add_executable(clock ${SOURCES})
set_target_properties(clock PROPERTIES COMPILE_FLAGS "-fPIC -std=c++20")
# Set RPATH for the executable
set_target_properties(clock PROPERTIES INSTALL_RPATH "$ORIGIN")

target_include_directories(clock  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/lib/include" )
target_link_libraries(clock uarm)


include(FindThreads)
find_package( Threads REQUIRED )
link_libraries(Threads::Threads)
add_compile_options(${CMAKE_THREAD_LIBS_INIT})


#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
