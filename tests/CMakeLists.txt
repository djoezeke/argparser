# ===================================================================
# Copyright © 2025 Sackey Ezekiel Etrue
# Distributed under the terms of the MIT license.
# ===================================================================

#--------------------------------------------------------------------
# Test Functions
#--------------------------------------------------------------------

# add a new target which is a test
# example: argparser_add_test(main-tests SOURCES main.test.cpp DEPENDS Argparser::Argparser)
function(argparser_add_test target)

    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS" ${ARGN})
    if(NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling argparser_add_test: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    if(NOT THIS_SOURCES)
        message(FATAL_ERROR "argparser_add_test requires SOURCES")
    endif()

    # set a source group for the source files
    source_group("" FILES ${THIS_SOURCES})

    # create the target
    add_executable(${target} ${THIS_SOURCES})

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Tests")

    set_target_properties(${target} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} # set the Visual Studio startup path for debugging
        VS_DEBUGGER_COMMAND_ARGUMENTS "-b" # Break into debugger

        XCODE_GENERATE_SCHEME ON # Required to set arguments
        XCODE_SCHEME_ARGUMENTS "-b" # Break into debugger
    )

    # link the target to its dependencies
    target_link_libraries(${target} PRIVATE ${THIS_DEPENDS})

    # Enable support for UTF-8 characters in source code
    if(MSVC)
        target_compile_options(${target} PRIVATE /utf-8)
    endif()

    # Add the test
    add_test(NAME ${target}
             COMMAND ${target}
             WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endfunction()

#--------------------------------------------------------------------
# Manually or Recursively add Tests
#--------------------------------------------------------------------

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")
argparser_add_test(argparser-tests
                   SOURCES ${TEST_SOURCES}
                   DEPENDS Argparser::Argparser)
