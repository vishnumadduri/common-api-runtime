cmake_minimum_required (VERSION 2.8.1)

PROJECT(libcommonapi)

# create the commandline variable:
SET(PACKAGE_VERSION "r0" CACHE STRING "rpm packet version")
SET(BUILD_RELEASE "No" CACHE STRING "Build release vesrion")

SET( ${PROJECT_NAME}_MAJOR_VERSION 2 )
SET( ${PROJECT_NAME}_MINOR_VERSION 1 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 5 ) 

set(COMPONENT_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})

#package version is needed for building rpm packets
#set(PACKAGE_VERSION ${COMPONENT_VERSION})


set(COMMONAPI_API_HEADER_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION})

message("RPM packet version set to ${PACKAGE_VERSION}")
message("Build type Release: ${BUILD_RELEASE}")
##################### RPM CONFIG ########################
SET( LICENSE "MPLv2" )
#########################################################
message("------------------------------") 
message("${CMAKE_CXX_FLAGS}") 

if(${BUILD_RELEASE} STREQUAL "Yes")
 STRING(REGEX REPLACE "-g " "-s " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
 #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
 message("Building Release Version !") 
endif(${BUILD_RELEASE} STREQUAL "Yes")

message("${CMAKE_CXX_FLAGS}") 

message("------------------------------") 

SET( prefix ${CMAKE_INSTALL_PREFIX})
SET( libdir "\${exec_prefix}/lib" )
SET( includedir "\${exec_prefix}/include" )

FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(Threads REQUIRED)

###############################################################################
# is using the 'pkg-config' tool
##############################################################################
# general stuff

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -DCOMMONAPI_INTERNAL_COMPILATION")


message("Compiler options: ${CMAKE_CXX_FLAGS}") 

OPTION(BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION(ENABLE_BUILD_DOC "Set to ON to build Documentation" OFF)

ADD_SUBDIRECTORY( src/CommonAPI )


##############################################################################
# installing files

INSTALL(FILES CommonAPI.pc
	DESTINATION /usr/lib/pkgconfig 
	COMPONENT base)


CONFIGURE_FILE(commonapi.spec.in commonapi.spec)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/CommonAPI.pc.in ${CMAKE_CURRENT_SOURCE_DIR}/CommonAPI.pc)
CONFIGURE_FILE(CommonAPI-uninstalled.pc.in CommonAPI-uninstalled.pc)
CONFIGURE_FILE(doxygen.cfg Doxygen)



