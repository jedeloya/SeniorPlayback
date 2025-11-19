cmake_minimum_required(VERSION 3.16)

project(minimalGStreamer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES ON)
set(CMAKE_OSX_ARCHITECTURES x86_64)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

# Remove duplicate libraries
list(REMOVE_DUPLICATES GSTREAMER_LIBRARIES)
list(REMOVE_DUPLICATES GLIB_LIBRARIES)
list(REMOVE_DUPLICATES GOBJECT_LIBRARIES)

message(STATUS "GStreamer include dirs: ${GSTREAMER_INCLUDE_DIRS}")
message(STATUS "GStreamer libs:         ${GSTREAMER_LIBRARIES}")
message(STATUS "GStreamer cflags:       ${GSTREAMER_CFLAGS_OTHER}")


add_executable(minimalGStreamer main.cpp)

target_include_directories(minimalGStreamer PRIVATE
    ${GSTREAMER_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GOBJECT_INCLUDE_DIRS}
)

target_compile_options(minimalGStreamer PRIVATE ${GSTREAMER_CFLAGS_OTHER})

# Library directories (if pkg-config only gives -l names)
target_link_directories(minimalGStreamer PRIVATE
    ${GSTREAMER_LIBRARY_DIRS}
    ${GLIB_LIBRARY_DIRS}
    ${GOBJECT_LIBRARY_DIRS}
)

target_link_libraries(minimalGStreamer PRIVATE
    ${GSTREAMER_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GOBJECT_LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS minimalGStreamer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

