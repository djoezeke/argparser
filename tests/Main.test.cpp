#include <cassert>
#include <iostream>

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

    std::cout << "All argparser tests passed\n";
    return 0;
}
