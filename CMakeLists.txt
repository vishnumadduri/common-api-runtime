cmake_minimum_required (VERSION 2.8.1)

PROJECT(libcommonapi)

# OS
set(DL_LIBRARY "")
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(DL_LIBRARY "dl")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# create the commandline variable:
SET(RPM_PACKAGE_VERSION "r0" CACHE STRING "rpm packet version")
SET(BUILD_RELEASE "No" CACHE STRING "Build release vesrion")

SET( ${PROJECT_NAME}_MAJOR_VERSION 2 )
SET( ${PROJECT_NAME}_MINOR_VERSION 1 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 6 ) 

set(COMPONENT_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})
set(COMMONAPI_API_HEADER_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION})

SET(PACKAGE_VERSION "${COMPONENT_VERSION}")

message("RPM packet version set to ${RPM_PACKAGE_VERSION}")
message("Build type Release: ${BUILD_RELEASE}")
##################### RPM CONFIG ########################
SET( LICENSE "MPLv2" )
#########################################################

if(${BUILD_RELEASE} STREQUAL "Yes")
 STRING(REGEX REPLACE "-g " "-s " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
 #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
 message("Building Release Version !") 
endif(${BUILD_RELEASE} STREQUAL "Yes")

SET( prefix "/usr")
SET( exec_prefix "${prefix}")
SET( libdir "${prefix}/lib" )
SET( datadir "${prefix}/share" )
SET( includedir "${exec_prefix}/include" )
set(cmakeinstalldir "${libdir}/cmake")
set(ESCAPED_API_HEADER_VERSION "@ESCAPED_API_HEADER_VERSION@")

message("${prefix}")
message("${exec_prefix}")
message("${libdir}")
message("${includedir}")

FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(Threads REQUIRED)

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

