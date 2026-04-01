#include <iostream>

#include "../argparser.hpp"

int main()
{
    argparser::HelpFormatter fmt(72, "format");
    fmt.description("Demonstrates custom help sections and layout.")
        .prefix_info("Argparser formatter demo")
        .suffix_info("End of help output")
        .line_separator('-')
        .format(argparser::HelpFormatter::Format::Plain);

    auto &general = fmt.add_section("general", "Core options", argparser::HelpFormatter::Section::Type::Arguments);
    general.add_item("-h, --help", "", "show help and exit", false);
    general.add_item("-v, --verbose", "", "enable verbose logging", false);

    auto &examples = fmt.add_section("examples", "Common usage", argparser::HelpFormatter::Section::Type::Examples);
    examples.add_example("format input.txt --verbose", "Run the formatter demo");

    std::cout << fmt.format_help();
    return 0;
}
