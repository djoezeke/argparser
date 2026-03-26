# ===================================================================
# Copyright © 2025 Sackey Ezekiel Etrue
# Distributed under the terms of the MIT license.
# ===================================================================

#--------------------------------------------------------------------
# Test Functions
#--------------------------------------------------------------------

# add a new target which is a test
# example: argparser_add_test(main-tests main.test.cpp ... Argparser::Argparser )
function(argparser_add_test target SOURCES DEPENDS)

    # set a source group for the source files
    source_group("" FILES ${SOURCES})

    # create the target
    add_executable(${target} ${SOURCES})

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Tests")

    set_target_properties(${target} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} # set the Visual Studio startup path for debugging
        VS_DEBUGGER_COMMAND_ARGUMENTS "-b" # Break into debugger

        XCODE_GENERATE_SCHEME ON # Required to set arguments
        XCODE_SCHEME_ARGUMENTS "-b" # Break into debugger
    )

    # link the target to its dependencies
    # target_link_libraries(${target} PRIVATE ${DEPENDS} Catch2::Catch2WithMain)

    # If coverage is enabled for MSVC and we are linking statically, use /WHOLEARCHIVE
    # to make sure the linker doesn't discard unused code sections before coverage can be measured
    if(MYJSON_ENABLE_COVERAGE AND MSVC AND NOT BUILD_SHARED_LIBS)
        foreach(DEPENDENCY ${DEPENDS})
            target_link_options(${target} PRIVATE $<$<CONFIG:DEBUG>:/WHOLEARCHIVE:$<TARGET_LINKER_FILE:${DEPENDENCY}>>)
        endforeach()
    endif()

    # Delay test registration when cross compiling to avoid running crosscompiled app on host OS
    if(CMAKE_CROSSCOMPILING)
        set(CMAKE_CATCH_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
    endif()

    # Enable support for UTF-8 characters in source code
    if(MSVC)
        target_compile_options(${target} PRIVATE /utf-8)
    endif()

    # Add the test
    add_test(${target} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endfunction()

#--------------------------------------------------------------------
# Manually or Recursively add Tests
#--------------------------------------------------------------------

# Build Test for all C Files
file(GLOB C_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")
argparser_add_test(argparser-c-tests "${C_TEST_SRC}" "Argparser::Argparser")

# Build Test for all C++ Files
file(GLOB CPP_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")
argparser_add_test(argparser-cpp-tests "${CPP_TEST_SRC}" "Argparser::Argparser")
