#include <iostream>

#include "../argparser.hpp"

int main(int argc, char **argv)
{
    argparser::ArgumentParser parser("basic", "Header-only C++ parser with Python-style API");
    parser.add_argument("input").help("input file path");
    parser.add_argument("-v", "--verbose").action("store_true").help("enable verbose output");
    parser.add_argument("-n", "--number").default_value(1).help("repeat count");
    parser.add_argument("-t", "--tag").action("append").help("add a tag (repeatable)");

    try
    {
        const auto &ns = parser.parse_args(argc, argv);
        std::cout << "input=" << ns.get<std::string>("input") << "\n";
        std::cout << "verbose=" << (ns.get<bool>("verbose") ? "true" : "false") << "\n";
        std::cout << "number=" << ns.get<int>("number") << "\n";

        if (ns.provided("tag"))
        {
            const auto tags = ns.getlist<std::string>("tag");
            for (const auto &tag : tags)
            {
                std::cout << "tag=" << tag << "\n";
            }
        }
    }
    catch (const argparser::HelpError &)
    {
        parser.print_help();
        return 0;
    }
    catch (const argparser::UsageError &)
    {
        parser.print_usage();
        return 0;
    }
    catch (const argparser::ArgumentError &e)
    {
        std::cerr << e.what() << "\n\n";
        parser.print_usage(std::cerr);
        parser.print_help(std::cerr);
        return 1;
    }

    return 0;
}