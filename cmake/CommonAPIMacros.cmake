include(CMakeParseArguments)

if(CommonAPI_GENERATOR_JAR)
    find_package(Java COMPONENTS Runtime REQUIRED QUIET)
    find_package(CommonAPI-DBus QUIET)

    function(common_api_generate_files)
        function(mktmpdir OUTVAR)
            while(NOT TEMP_DESTINATION OR EXISTS ${TEMP_DESTINATION})
                string(RANDOM LENGTH 16 TEMP_DESTINATION)
                set(TEMP_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${TEMP_DESTINATION}")
            endwhile()

           file(MAKE_DIRECTORY ${TEMP_DESTINATION})

           set(${OUTVAR} ${TEMP_DESTINATION} PARENT_SCOPE)
        endfunction()

        set(options DBUS)
        set(oneValueArgs TARGET DESTINATION HEADER_TEMPLATE)
        set(multiValueArgs FIDLS FIDL_DEPENDS)

        cmake_parse_arguments(PARAMS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        if(NOT PARAMS_FIDLS)
            message(FATAL_ERROR "FIDLS must be specified")
        endif()

        if(NOT PARAMS_TARGET)
            message(FATAL_ERROR "TARGET must be specified")
        endif()

        if(PARAMS_DBUS)
            if(NOT CommonAPI-DBus_FOUND)
                message(FATAL_ERROR "CommonAPI-DBus is required to enable D-Bus support")
            endif()
            list(APPEND ARGS -dbus)
        endif()

        if(PARAMS_HEADER_TEMPLATE)
            list(APPEND ARGS -pref ${PARAMS_HEADER_TEMPLATE})
        endif()

        if(NOT IS_ABSOLUTE PARAMS_DESTINATION)
            set(PARAMS_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${PARAMS_DESTINATION})
        endif()

        foreach(FIDL ${PARAMS_FIDLS})
            get_filename_component(FIDL_PATH ${FIDL} ABSOLUTE)

            # Run configure_file on the .fidl - this forces cmake to reexecute its
            # configure phase if the input file changes.
            string(MD5 ${FIDL_PATH} FIDL_CHECKSUM)
            configure_file(${FIDL_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${FIDL_CHECKSUM}.fidl.done)

            list(APPEND FIDLS ${FIDL_PATH})
        endforeach()

        foreach(FIDL_DEPEND ${PARAMS_FIDL_DEPENDS})
            string(MD5 ${FIDL_PATH} FIDL_CHECKSUM)
            configure_file(${FIDL_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${FIDL_CHECKSUM}.fidl.done)
        endforeach()

        message(STATUS "Determining list of generated files for ${PARAMS_FIDLS}")

        # Store result in a temporary directory so a clean file listing can be retrieved via globbing
        mktmpdir(TEMP_DESTINATION)
	
        #execute_process(COMMAND ${CommonAPI_GENERATOR_JAR} -dest ${TEMP_DESTINATION} ${ARGS} ${FIDLS}
	execute_process(COMMAND ${Java_JAVA_EXECUTABLE} -jar ${CommonAPI_GENERATOR_JAR} -dest ${TEMP_DESTINATION} ${ARGS} ${FIDLS}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE EXIT_CODE
                        OUTPUT_VARIABLE GENERATOR_OUTPUT
                        ERROR_VARIABLE GENERATOR_OUTPUT
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_STRIP_TRAILING_WHITESPACE)

        file(GLOB_RECURSE SOURCES RELATIVE ${TEMP_DESTINATION} ${TEMP_DESTINATION}/*.cpp ${TEMP_DESTINATION}/*.h)
        file(REMOVE_RECURSE ${TEMP_DESTINATION})

        if(EXIT_CODE)
            message(FATAL_ERROR "Failed to generate files from FIDL:\n ${GENERATOR_OUTPUT}")
        endif()

        foreach(SOURCE ${SOURCES})
            list(APPEND MOVED_SOURCES ${PARAMS_DESTINATION}/${SOURCE})
        endforeach()
        set_source_files_properties(${MOVED_SOURCES} PROPERTIES GENERATED TRUE)

        add_custom_command(OUTPUT ${MOVED_SOURCES}
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${PARAMS_DESTINATION}
			   COMMAND ${Java_JAVA_EXECUTABLE} -jar ${CommonAPI_GENERATOR_JAR} -dest ${PARAMS_DESTINATION} ${ARGS} ${FIDLS}
                           #COMMAND ${CommonAPI_GENERATOR_JAR} -dest ${PARAMS_DESTINATION} ${ARGS} ${FIDLS}
                           DEPENDS ${PARAMS_FIDLS} ${PARAMS_FIDL_DEPENDS})
        add_custom_target(${PARAMS_TARGET} DEPENDS ${MOVED_SOURCES})

        set(INCLUDES ${PARAMS_DESTINATION} ${CommonAPI_INCLUDES})
        set(LIBRARIES ${CommonAPI_LIBRARIES})
        if(PARAMS_DBUS)
            list(APPEND INCLUDES ${CommonAPI-DBus_INCLUDE_DIRS})
            list(APPEND LIBRARIES ${CommonAPI-DBus_LIBRARIES})
        endif()

        set(${PARAMS_TARGET}_INCLUDE_DIRS ${INCLUDES} PARENT_SCOPE)
        set(${PARAMS_TARGET}_LIBRARIES ${LIBRARIES} PARENT_SCOPE)
        set(${PARAMS_TARGET}_GENERATED_SOURCES ${MOVED_SOURCES} PARENT_SCOPE)
    endfunction(common_api_generate_files)
endif()
