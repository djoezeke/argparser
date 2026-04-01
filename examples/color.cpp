#include <iostream>

#include "../argparser.hpp"

int main(int argc, char **argv)
{
    argparser::ArgumentParser parser("color", "Colored help output demo");
    parser.formatter().use_color(true);
    parser.add_argument("--verbose").action("store_true").help("enable verbose output");
    parser.add_argument("input").help("input file");

    try
    {
        const auto &ns = parser.parse_args(argc, argv);
        std::cout << "input=" << ns.get<std::string>("input") << "\n";
        std::cout << "verbose=" << (ns.get<bool>("verbose") ? "true" : "false") << "\n";
    }
    catch (const argparser::HelpError &)
    {
        parser.print_help(std::cout);
        return 0;
    }
    catch (const argparser::ArgumentError &e)
    {
        std::cerr << e.what() << "\n\n";
        parser.print_usage(std::cerr);
        return 1;
    }

    return 0;
}
