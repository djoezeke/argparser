#define ARGPARSER_IMPLEMENTATION
#include "../argparser.h"

/**
 * Main function demonstrating the usage of the ArgumentParser.
 *
 * @param argc The argument count.
 * @param argv The argument vector.
 * @return Exit status.
 *
 * Example usage:
 * ./my_program -v -s -c 5
 */
int main(int argc, char const *argv[])
{
    ArgumentParser_t *argparser;
    argparser_initialize(argparser, "my_program", "Usage: my_program [options]", "This is a sample program.", "Epilog message");

    // // Add flag arguments
    // ArgumentParserAddFlag(&argparser, 'v', "verbose", "Enable verbose mode");
    // ArgumentParserAddFlag(&argparser, 's', "store", "Save file Name");

    // // Add keyword argument
    // ArgumentParserAddKwarg(&argparser, 'c', "count", 0, NULL, "Number of times");

    // // Parse the command-line arguments
    // ArgumentParserParseArgs(&argparser, argc, argv);

    // // Retrieve the values of the arguments
    // int verbose = ArgumentParserGetFlag(&argparser, "verbose");
    // int store = ArgumentParserGetFlag(&argparser, "store");
    // int help = ArgumentParserGetFlag(&argparser, "help");
    // const char *count = ArgumentParserGetKwarg(&argparser, "count");

    // // Print the help message if the help flag is set
    // if (help)
    //     ArgumentParserPrintHelp(&argparser);

    // // Print the values of the arguments
    // if (count)
    //     printf("Count: %s\n", count);
    // if (store)
    //     printf("Store: %d\n", store);
    // if (verbose)
    //     printf("Verbose: %d\n", verbose);

    // Free the memory allocated for the argparser
    argparser_delete(argparser);

    return 0;
}

/**
 * LICENSE: Public Domain (www.unlicense.org)
 *
 * Copyright (c) 2025 Sackey Ezekiel Etrue
 *
 * This is free and unencumbered software released into the public domain.
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
 * software, either in source code form or as a compiled binary, for any purpose,
 * commercial or non-commercial, and by any means.
 * In jurisdictions that recognize copyright laws, the author or authors of this
 * software dedicate any and all copyright interest in the software to the public
 * domain. We make this dedication for the benefit of the public at large and to
 * the detriment of our heirs and successors. We intend this dedication to be an
 * overt act of relinquishment in perpetuity of all present and future rights to
 * this software under copyright law.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */