cmake_minimum_required(VERSION 3.16...4.1.1 FATAL_ERROR)

#--------------------------------------------------------------------
# Project and version.
#--------------------------------------------------------------------

set (ARGPARSER_VERSION_MAJOR 0)
set (ARGPARSER_VERSION_MINOR 1)
set (ARGPARSER_VERSION_PATCH 1)
set (ARGPARSER_VERSION_STRING ${ARGPARSER_VERSION_MAJOR}.${ARGPARSER_VERSION_MINOR}.${ARGPARSER_VERSION_PATCH})

project(argparser
        VERSION ${ARGPARSER_VERSION_STRING} 
        DESCRIPTION "Argument parser for C/C++"
        HOMEPAGE_URL "https://github.com/djoezeke/argparser"
        LANGUAGES  C CXX
        )

include(GNUInstallDirs)

#--------------------------------------------------------------------
# Preparations depending on the specified build options  
#--------------------------------------------------------------------

# Check if Argparser is the main project
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(ARGPARSER_IS_TOP_LEVEL ON)
else()
  set(ARGPARSER_IS_TOP_LEVEL OFF)
endif()

# Set required C/C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Add UTF-8 encoding support for MSVC compiler.
# This ensures that the MSVC compiler interprets source files as UTF-8 encoded,
# which is useful for projects containing non-ASCII characters in source files.
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#--------------------------------------------------------------------
# Configure variables for the library   
#--------------------------------------------------------------------

option(ARGPARSER_BUILD_EXAMPLES "Build the ${PROJECT_NAME} example applications" ${ARGPARSER_IS_TOP_LEVEL})
option(ARGPARSER_BUILD_TESTS "Build the ${PROJECT_NAME} test programs" ${ARGPARSER_IS_TOP_LEVEL})

option(ARGPARSER_ENABLE_INSTALL "Enable installation." ${ARGPARSER_IS_TOP_LEVEL})
option(ARGPARSER_ENABLE_DOXYGEN "Build documentation with Doxygen." ${ARGPARSER_IS_TOP_LEVEL})
option(ARGPARSER_ENABLE_WARNING "Enable warning messages." ${ARGPARSER_IS_TOP_LEVEL})
option(ARGPARSER_ENABLE_PACKING "Enable packing with CPack." ${ARGPARSER_IS_TOP_LEVEL})

#--------------------------------------------------------------------
# Configurations
#--------------------------------------------------------------------

# Default to build type "Release" unless tests are being built
if(NOT CMAKE_BUILD_TYPE)
	if (NOT ARGPARSER_BUILD_TESTS)
		message(STATUS "No build type selected, default to Release")
		set(CMAKE_BUILD_TYPE "Release")
	else()
		message(STATUS "No build type selected but tests are being built, default to Debug")
		set(CMAKE_BUILD_TYPE "Debug")
	endif()
endif()

# Output directories for a build tree
if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(ARGPARSER_ENABLE_WARNING)

endif()

#--------------------------------------------------------------------
# Build library targets
#--------------------------------------------------------------------

add_library(Argparser INTERFACE)
add_library(Argparser::Argparser ALIAS Argparser)

target_include_directories(Argparser 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#--------------------------------------------------------------------
# Add Project subdirectories
#--------------------------------------------------------------------

# Build the example apps   
if(ARGPARSER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Build the documentation   
if(ARGPARSER_ENABLE_DOXYGEN)
    add_subdirectory(docs)
endif()

# Build the test apps   
if(ARGPARSER_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)

    # Set the unit test app project as the Visual Studio startup project
    # if the target compiler is some version of Microsoft Visual C++ and
    # if this project is the main project.
    if(MSVC AND CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
      set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT argparser_tests)
    endif()
endif()

#--------------------------------------------------------------------
# Configure package.
#--------------------------------------------------------------------

include(CMakePackageConfigHelpers)

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/ArgparserConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/ArgparserConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Argparser"
)


#--------------------------------------------------------------------
# Install package.
#--------------------------------------------------------------------

if(ARGPARSER_ENABLE_INSTALL)
	install(TARGETS Argparser EXPORT ArgparserTargets)

  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/argparser.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/argparser.h)
    install(FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/argparser.h
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
  endif()

	install(EXPORT ArgparserTargets
		FILE ArgparserTargets.cmake
		NAMESPACE Argparser::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Argparser"
	)

    install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/ArgparserConfig.cmake"
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Argparser"
	)

endif()


#--------------------------------------------------------------------
# Package with CPack.
#--------------------------------------------------------------------

if(ARGPARSER_ENABLE_PACKING)

    if(WIN32 AND NOT CYGWIN)
        set(CPACK_GENERATOR NSIS ZIP)
    endif()
    if(APPLE)
        set(CPACK_GENERATOR DragNDrop TGZ)
    endif()
    if (UNIX AND NOT APPLE AND NOT WIN32)
        set(CPACK_GENERATOR DEB TGZ)
    endif ()

  set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
  set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
  set(CPACK_PACKAGE_VENDOR "${PROJECT_NAME} developers")
  set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
  set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
  set(CPACK_PACKAGE_MAINTAINER "Sackey Ezekiel Etrue")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

  set(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
  set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
  set(CPACK_DEBIAN_PACKAGE_DEBUG OFF)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6-dev")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
  set(CPACK_DEBIAN_PACKAGE_SUGGESTS "cmake, pkg-config, pkg-conf")

  set(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
  set(CPACK_RPM_PACKAGE_SUGGESTS "${CPACK_DEBIAN_PACKAGE_SUGGESTS}")

  set(CPACK_DEB_COMPONENT_INSTALL ON)
  set(CPACK_RPM_COMPONENT_INSTALL ON)
  set(CPACK_NSIS_COMPONENT_INSTALL ON)

    include(CPack)

endif()

#--------------------------------------------------------------------
# Uninstall Files
#--------------------------------------------------------------------

if(NOT TARGET uninstall)

    configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )

endif()
