cmake_minimum_required(VERSION 3.16)

project(minimalGStreamer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

message(STATUS "GStreamer include dirs: ${GSTREAMER_INCLUDE_DIRS}")
message(STATUS "GStreamer libs:         ${GSTREAMER_LIBRARIES}")
message(STATUS "GStreamer cflags:       ${GSTREAMER_CFLAGS_OTHER}")

# Add include dirs
include_directories(${GSTREAMER_INCLUDE_DIRS})

# Add library path manually

link_directories(${GSTREAMER_LIBRARY_DIRS})

add_executable(minimalGStreamer main.cpp)

target_include_directories(minimalGStreamer PRIVATE
    ${GSTREAMER_INCLUDE_DIRS}
)

target_link_libraries(minimalGStreamer
    ${GSTREAMER_LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS minimalGStreamer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
