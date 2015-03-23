cmake_minimum_required (VERSION 2.8.1)

PROJECT(libcommonapi)

# version of CommonAPI
SET( LIBCOMMONAPI_MAJOR_VERSION 3 )
SET( LIBCOMMONAPI_MINOR_VERSION 0 )
SET( LIBCOMMONAPI_PATCH_VERSION 0 )

message(STATUS "Project name: ${PROJECT_NAME}")

set(COMPONENT_VERSION ${LIBCOMMONAPI_MAJOR_VERSION}.${LIBCOMMONAPI_MINOR_VERSION}.${LIBCOMMONAPI_PATCH_VERSION})
set(COMMONAPI_API_HEADER_VERSION ${LIBCOMMONAPI_MAJOR_VERSION}.${LIBCOMMONAPI_MINOR_VERSION}) # used in *.cmake.in

SET(PACKAGE_VERSION "${COMPONENT_VERSION}") # used in *.cmake.in

set(GTEST_ROOT "n/a" CACHE STRING "Path to root folder of googletest. Must be set for building the tests.")

# OS
set(DL_LIBRARY "")
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(DL_LIBRARY "dl")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

##############################################################################

# define standard installation path in case CMAKE_INSTALL_PREFIX is not set
if ("${CMAKE_INSTALL_PREFIX}" STREQUAL "")
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
message(STATUS "CMAKE_INSTALL_PREFIX set to: ${CMAKE_INSTALL_PREFIX}")

# create the commandline variables:
OPTION(BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
message(STATUS "BUILD_SHARED_LIBS is set to value: ${BUILD_SHARED_LIBS}")
SET(RPM_PACKAGE_VERSION "r0" CACHE STRING "rpm packet version") # used in e.g. commonapi.spec.in

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
       "Choose the type of build, options are: Debug Release." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_BUILD_TYPE_FOR_SPEC_IN "")
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_FOR_SPEC_IN)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include/CommonAPI-${COMMONAPI_API_HEADER_VERSION} CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR cmake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/CommonAPI-${COMPONENT_VERSION})
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

message(STATUS "RPM packet version set to ${RPM_PACKAGE_VERSION}")
##################### RPM CONFIG ########################
SET( LICENSE "MPLv2" )
#########################################################

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

##############################################################################

# CommonAPI build section

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -DCOMMONAPI_INTERNAL_COMPILATION")

message(STATUS "Compiler options: ${CMAKE_CXX_FLAGS}") 

include_directories(
    include
)

file(GLOB CAPI_SRCS "src/CommonAPI/*.cpp")
add_library(CommonAPI ${CAPI_SRCS})
target_link_libraries(CommonAPI ${DL_LIBRARY})
set_target_properties(CommonAPI PROPERTIES VERSION ${LIBCOMMONAPI_MAJOR_VERSION}.${LIBCOMMONAPI_MINOR_VERSION}.${LIBCOMMONAPI_PATCH_VERSION} SOVERSION ${LIBCOMMONAPI_MAJOR_VERSION} LINKER_LANGUAGE C)

##############################################################################
# configure files

CONFIGURE_FILE(commonapi.spec.in commonapi.spec)

##############################################################################
# installing files

# for installation of CommonAPI header files
file (GLOB_RECURSE CommonAPI_INCLUDE_INSTALL_FILES "include/CommonAPI/*.hpp")
set_target_properties (CommonAPI PROPERTIES PUBLIC_HEADER "${CommonAPI_INCLUDE_INSTALL_FILES}")

# install CommonAPI library including headers
install(TARGETS CommonAPI
        EXPORT CommonAPITargets
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/CommonAPI"
)

##############################################################################
# exporting, configuring and installing of cmake files

# Add all targets to the build-tree export set
export(TARGETS CommonAPI
  FILE "${PROJECT_BINARY_DIR}/CommonAPITargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE CommonAPI)

# Create the CommonAPIConfig.cmake and CommonAPIConfigVersion files ...
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")

# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfig.cmake.in
  "${PROJECT_BINARY_DIR}/CommonAPIConfig.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${COMMONAPI_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CommonAPIConfig.cmake" @ONLY)

# ... for both
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/CommonAPIConfigVersion.cmake" @ONLY)

# Install the CommonAPIConfig.cmake and CommonAPIConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CommonAPIConfig.cmake"
  "${PROJECT_BINARY_DIR}/CommonAPIConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}")

# Install the export set for use with the install-tree
install(EXPORT CommonAPITargets DESTINATION
  "${INSTALL_CMAKE_DIR}")

##############################################################################
# maintainer-clean
add_custom_target(maintainer-clean COMMAND rm -rf *)

##############################################################################
# build documentation
FIND_PACKAGE(Doxygen)
FIND_PROGRAM(ASCIIDOC_PATH asciidoc)
if (NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen is not installed. Documentation can not be built.")
else()
    # set configuration variables for doxygen.in
    set(PROJECT "GENIVI IPC Common API C++")
    set(DOCDIR doc)
    set(SRCDIR .)
    set(GENERATE_HTML YES)
    set(GENERATE_HTMLHELP NO)
    set(GENERATE_CHI NO)
    set(GENERATE_LATEX NO)
    set(GENERATE_PDF NO)
    set(GENERATE_RTF NO)
    set(GENERATE_MAN NO)
    set(GENERATE_XML NO)

    configure_file(doxygen.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_target(doc)

    add_custom_target(doxygen-doc
                      COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                      SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)

    add_dependencies(doc doxygen-doc)

    if ("${ASCIIDOC_PATH}" STREQUAL "ASCIIDOC_PATH-NOTFOUND")
        message(STATUS "asciidoc is not installed. Readme can not be built.")
    else()
        message(STATUS "asciidoc found")
        add_custom_command(TARGET doc
                           POST_BUILD
                           COMMAND asciidoc
                                   -a version=${PACKAGE_VERSION}
                                   -b html
                                   -o doc/html/README.html 
                                   ${PROJECT_BINARY_DIR}/../README)
    endif()
endif()

##############################################################################
# Test section
##############################################################################

##############################################################################
# google test

# check for set environment variable
if(${GTEST_ROOT} STREQUAL "n/a")
    message(STATUS "GTEST_ROOT is not defined. For building the tests environment variable
             GTEST_ROOT has to be defined. Tests can not be built.")
    # early exit
    return()  # test can not be build -> make commands build_tests and check are not available
else()
    message(STATUS "GTEST_ROOT is set. gtest root path set to ${GTEST_ROOT}")
endif()

# build google test as static library (always) -> therefore deactivate BUILD_SHARED_LIBS in case it is active
set(BUILD_SHARED_LIBS_AUTOMATIC_OFF 0)
if ("${BUILD_SHARED_LIBS}" STREQUAL "ON")
    set(BUILD_SHARED_LIBS OFF)
    set(BUILD_SHARED_LIBS_AUTOMATIC_OFF 1)
endif()
add_subdirectory(${GTEST_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)
if ("${BUILD_SHARED_LIBS_AUTOMATIC_OFF}" STREQUAL "1")
    set(BUILD_SHARED_LIBS ON)
    set(BUILD_SHARED_LIBS_AUTOMATIC_OFF 0)
endif()

##############################################################################
# build tests

enable_testing()

add_custom_target(build_tests)

set(CMAKE_CTEST_COMMAND ctest -V)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

add_dependencies(check build_tests)

##############################################################################
# add test directory

add_subdirectory( src/test EXCLUDE_FROM_ALL )
