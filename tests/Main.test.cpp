#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../argparser.hpp"

int main()
{
    {
        argparser::ArgumentParser parser("test");
        parser.add_argument("file");
        parser.add_argument("-v", "--verbose").action("store_true");
        parser.add_argument("-n", "--number").default_value(7);

        const char *argv[] = {"test", "input.txt", "-v"};
        const auto &ns = parser.parse_args(3, const_cast<char **>(argv));

        assert(ns.get<std::string>("file") == "input.txt");
        assert(ns.get<bool>("verbose") == true);
        assert(ns.get<int>("number") == 7);
    }

    {
        argparser::ArgumentParser parser("test");
        parser.add_argument("-t", "--tag").action("append");

        const char *argv[] = {"test", "--tag", "a", "--tag", "b"};
        const auto &ns = parser.parse_args(5, const_cast<char **>(argv));

        const auto tags = ns.getlist<std::string>("tag");
        assert(tags.size() == 2);
        assert(tags[0] == "a");
        assert(tags[1] == "b");
    }

    {
        argparser::ArgumentParser parser("test");
        parser.add_argument("-v", "--verbose").action("count");

        const char *argv[] = {"test", "-vvv"};
        const auto &ns = parser.parse_args(2, const_cast<char **>(argv));
        assert(ns.get<int>("verbose") == 3);
    }

    {
        argparser::ArgumentParser parser("test");
        parser.add_argument("number");

        const char *argv[] = {"test", "-42"};
        const auto &ns = parser.parse_args(2, const_cast<char **>(argv));
        assert(ns.get<int>("number") == -42);
    }

    {
        argparser::ArgumentParser parser("test");
        parser.add_argument("--delta");

        const char *argv[] = {"test", "--delta", "-3.5"};
        const auto &ns = parser.parse_args(3, const_cast<char **>(argv));
        assert(ns.get<double>("delta") == -3.5);
    }

    {
        argparser::ArgumentParser parser("test");
        parser.add_argument("--known").action("store_true");

        const char *argv[] = {"test", "--known", "--mystery"};
        const auto &ns = parser.parse_known_args(3, const_cast<char **>(argv));
        const auto &unknown = parser.unknown_args();

        assert(ns.get<bool>("known") == true);
        assert(unknown.size() == 1);
        assert(unknown[0] == "--mystery");
    }

    {
#ifdef _WIN32
        _putenv_s("ARGPARSER_TEST_PORT", "8080");
#else
        setenv("ARGPARSER_TEST_PORT", "8080", 1);
#endif

        argparser::ArgumentParser parser("test");
        parser.add_argument("--port").env("ARGPARSER_TEST_PORT");

        const char *argv[] = {"test"};
        const auto &ns = parser.parse_args(1, const_cast<char **>(argv));
        assert(ns.get<int>("port") == 8080);
    }

    {
        argparser::ArgumentParser parser("tool");
        parser.add_argument("-v", "--verbose").action("store_true");

        auto &run = parser.add_subcommand("run", "run tasks");
        run.add_argument("path");
        run.add_argument("--mode").choices({"fast", "safe"}).default_value("fast");

        const char *argv[] = {"tool", "-v", "run", "project", "--mode", "safe"};
        const auto &ns = parser.parse_args(6, const_cast<char **>(argv));

        assert(ns.get<bool>("verbose") == true);
        assert(ns.has_subcommand());
        assert(ns.subcommand() == "run");
        assert(ns.subcommand_namespace().get<std::string>("path") == "project");
        assert(ns.subcommand_namespace().get<std::string>("mode") == "safe");
    }

    {
        argparser::HelpFormatter fmt(60, "sample");
        fmt.description("A small formatter test.")
            .epilog("done")
            .line_separator("-")
            .format(argparser::HelpFormatter::Format::Plain);

        auto &section = fmt.add_section("options");
        section.add_item("-v, --verbose", "", "enable verbose mode", false);

        const auto help = fmt.format_help();
        assert(help.find("usage:") != std::string::npos);
        assert(help.find("options:") != std::string::npos);
        assert(help.find("enable verbose mode") != std::string::npos);
        assert(help.find("done") != std::string::npos);
    }

    {
        argparser::ArgumentParser parser("tool", "docs");
        parser.add_argument("--mode").choices({"fast", "safe"}).default_value("fast");
        parser.add_argument("--feature").nargs('?').implicit_value("enabled");

        const char *argv[] = {"tool", "--feature"};
        const auto &ns = parser.parse_args(2, const_cast<char **>(argv));

        assert(ns.get<std::string>("mode") == "fast");
        assert(ns.get<std::string>("feature") == "enabled");
    }

    std::cout << "All argparser tests passed\n";
    return 0;
}
