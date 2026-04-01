#include <cstdlib>
#include <iostream>
#include <string>

#include "../argparser.hpp"

int main(int argc, char **argv)
{
    argparser::ArgumentParser parser("env", "Read configuration from the environment");
    parser.add_argument("--host").default_value("127.0.0.1").help("server host");
    parser.add_argument("--port").env("ARGPARSER_PORT").default_value(3000).help("server port");

    try
    {
        const auto &ns = parser.parse_args(argc, argv);
        std::cout << "host=" << ns.get<std::string>("host") << "\n";
        std::cout << "port=" << ns.get<int>("port") << "\n";
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
