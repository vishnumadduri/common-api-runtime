cmake_minimum_required (VERSION 2.8.1)

PROJECT(libcommonapi)

# OS, pthread
set(DL_LIBRARY "")
set(PTHREAD_LIBRARY "")
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(DL_LIBRARY "dl")
        set(PTHREAD_LIBRARY "pthread")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# create the commandline variable:
SET(RPM_PACKAGE_VERSION "r0" CACHE STRING "rpm packet version") # used in e.g. commonapi.spec.in
SET(BUILD_RELEASE "No" CACHE STRING "Build release version")

# version of library
SET( LIB_MAJOR_VERSION 3 ) # used in e.g. sub cmake file, commonapi.spec.in
SET( LIB_MINOR_VERSION 0 ) # used in e.g. sub cmake file, commonapi.spec.in
SET( LIB_PATCH_VERSION 1 ) # used in e.g. sub cmake file, commonapi.spec.in

# version of CommonAPI
SET( ${PROJECT_NAME}_MAJOR_VERSION 2 )
SET( ${PROJECT_NAME}_MINOR_VERSION 1 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 6 ) 

set(COMPONENT_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})
set(COMMONAPI_API_HEADER_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}) # used in e.g. CommonAPI.pc.in, *.cmake.in

SET(PACKAGE_VERSION "${COMPONENT_VERSION}") # used in e.g. CommonAPI.pc.in, *.cmake.in

message("RPM packet version set to ${RPM_PACKAGE_VERSION}")
message("Build type Release: ${BUILD_RELEASE}")
##################### RPM CONFIG ########################
SET( LICENSE "MPLv2" )
#########################################################

if(${BUILD_RELEASE} STREQUAL "Yes")
 STRING(REGEX REPLACE "-g " "-s " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
 message("Building Release Version !") 
endif(${BUILD_RELEASE} STREQUAL "Yes")

SET( prefix "/usr") # used in e.g. CommonAPI.pc.in, *.cmake.in
SET( exec_prefix "${prefix}") # used in e.g. CommonAPI.pc.in, *.cmake.in
SET( libdir "${prefix}/lib" ) # used in e.g. CommonAPI.pc.in, *.cmake.in
SET( datadir "${prefix}/share" ) # used in e.g. *.cmake.in
SET( includedir "${exec_prefix}/include" ) # used in e.g. CommonAPI.pc.in, *.cmake.in
set(cmakeinstalldir "${libdir}/cmake") # used in e.g. *.cmake.in
set(ESCAPED_API_HEADER_VERSION "@ESCAPED_API_HEADER_VERSION@") # used in e.g. *.cmake.in

#FIND_PACKAGE(PkgConfig)
#FIND_PACKAGE(Threads REQUIRED)

###############################################################################
# is using the 'pkg-config' tool
##############################################################################
# general stuff

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -DCOMMONAPI_INTERNAL_COMPILATION")

message("Compiler options: ${CMAKE_CXX_FLAGS}") 

OPTION(BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION(ENABLE_BUILD_DOC "Set to ON to build Documentation" OFF)

include_directories(
    src
)

ADD_SUBDIRECTORY( src/CommonAPI )

CONFIGURE_FILE(commonapi.spec.in commonapi.spec)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/CommonAPI.pc.in ${CMAKE_CURRENT_SOURCE_DIR}/CommonAPI.pc)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfig.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfig.cmake @ONLY)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfigVersion.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfigVersion.cmake @ONLY)
CONFIGURE_FILE(CommonAPI-uninstalled.pc.in CommonAPI-uninstalled.pc)
CONFIGURE_FILE(doxygen.cfg Doxygen)

##############################################################################
# installing files
INSTALL(FILES CommonAPI.pc DESTINATION /usr/lib/pkgconfig COMPONENT base)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIMacros.cmake DESTINATION /usr/lib/cmake/CommonAPI-${COMPONENT_VERSION} )
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfig.cmake DESTINATION /usr/lib/cmake/CommonAPI-${COMPONENT_VERSION} )
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPIConfigVersion.cmake DESTINATION /usr/lib/cmake/CommonAPI-${COMPONENT_VERSION} )

##############################################################################
# clean-all
add_custom_target(clean-all COMMAND rm -rf *)

##############################################################################
# Test section
##############################################################################

##############################################################################
# google test

# check for set environment variable
if(DEFINED ENV{GTEST_ROOT})
    message("GTEST_ROOT is set. gtest found in $ENV{GTEST_ROOT}")
else()
    message("GTEST_ROOT is not defined. For building the tests environment variable
             GTEST_ROOT have to be defined. Tests can not be built.")
    # early exit
    return()  # test can not be build -> make commands build_tests and check are not available
endif()

set(BUILD_SHARED_LIBS OFF)
add_subdirectory($ENV{GTEST_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)
set(BUILD_SHARED_LIBS ON)

##############################################################################
# add test directory
add_subdirectory( src/test EXCLUDE_FROM_ALL )

add_dependencies(CommonAPI_Tests gtest)

##############################################################################
# build tests
add_custom_target(build_tests)
add_dependencies(build_tests CommonAPI_Tests)

add_custom_target(check COMMAND CommonAPI_Tests)
add_dependencies(check build_tests)

