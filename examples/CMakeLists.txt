# ===================================================================
# Copyright © 2025 Sackey Ezekiel Etrue
# Distributed under the terms of the MIT license.
# ===================================================================

#--------------------------------------------------------------------
# Example Function
#--------------------------------------------------------------------

# add a new target which is an example
# example: argparser_add_example(example
#                     SOURCES example.cpp
#                     DEPENDS Argparser::Argparser)
function(argparser_add_example target)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS" ${ARGN})
    if(NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling argparser_add_example: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    # set a source group for the source files
    source_group("" FILES ${THIS_SOURCES})

    # create the target
    add_executable(${target} ${THIS_SOURCES})

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Examples")

    # set the Visual Studio startup path for debugging
    set_target_properties(${target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # link the target to its dependencies
    target_link_libraries(${target} PRIVATE ${THIS_DEPENDS})

    # Enable support for UTF-8 characters in source code
    if(MSVC)
        target_compile_options(${target} PRIVATE /utf-8)
    endif()
endfunction()

#--------------------------------------------------------------------
# Recursively Add Examples
#--------------------------------------------------------------------

# Automatically add all .c samples in this folder
file(GLOB C_SAMPLE_SOURCES "*.c")
foreach(EXAMPLE_FILE ${C_SAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} NAME_WE)
	argparser_add_example("${EXAMPLE_NAME}"
						SOURCES "${EXAMPLE_FILE}"
						DEPENDS Argparser::ArgparserC)
endforeach()

# Automatically add all .cpp samples in this folder
file(GLOB CPP_SAMPLE_SOURCES "*.cpp")
foreach(EXAMPLE_FILE ${CPP_SAMPLE_SOURCES})
  	get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} NAME_WE)
	argparser_add_example("${EXAMPLE_NAME}"
						SOURCES "${EXAMPLE_FILE}"
						DEPENDS Argparser::Argparser)
endforeach()

# ===================================================================
