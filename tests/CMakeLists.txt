if(WIN32)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(argparser_tests basic_test.cpp)
target_link_libraries(argparser_tests PRIVATE Argparser::Argparser)

add_test(NAME argparser_tests COMMAND argparser_tests)
