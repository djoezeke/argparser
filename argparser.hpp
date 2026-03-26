#pragma once

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace argparser
{

    class ArgumentParserError : public std::runtime_error
    {
    public:
        explicit ArgumentParserError(const std::string &message)
            : std::runtime_error(message)
        {
        }
    };

    class HelpRequested : public std::runtime_error
    {
    public:
        HelpRequested()
            : std::runtime_error("help requested")
        {
        }
    };

    class Namespace
    {
    public:
        bool has(const std::string &name) const
        {
            return m_Data.find(name) != m_Data.end();
        }

        bool provided(const std::string &name) const
        {
            const auto it = m_Provided.find(name);
            return it != m_Provided.end() && it->second;
        }

        const std::vector<std::string> &raw_values(const std::string &name) const
        {
            const auto it = m_Data.find(name);
            if (it == m_Data.end())
            {
                throw ArgumentParserError("unknown argument: " + name);
            }
            return it->second;
        }

        template <typename T>
        T get(const std::string &name) const
        {
            const auto &vals = raw_values(name);
            if (vals.empty())
            {
                throw ArgumentParserError("argument has no value: " + name);
            }
            return convert<T>(vals.front());
        }

        template <typename T>
        std::vector<T> get_list(const std::string &name) const
        {
            const auto &vals = raw_values(name);
            std::vector<T> out;
            out.reserve(vals.size());
            for (const auto &v : vals)
            {
                out.emplace_back(convert<T>(v));
            }
            return out;
        }

    private:
        template <typename T>
        static T convert(const std::string &value)
        {
            if constexpr (std::is_same<T, std::string>::value)
            {
                return value;
            }
            else if constexpr (std::is_same<T, bool>::value)
            {
                std::string lowered = value;
                std::transform(lowered.begin(), lowered.end(), lowered.begin(),
                               [](unsigned char c)
                               { return static_cast<char>(std::tolower(c)); });
                if (lowered == "1" || lowered == "true" || lowered == "yes" || lowered == "on")
                {
                    return true;
                }
                if (lowered == "0" || lowered == "false" || lowered == "no" || lowered == "off")
                {
                    return false;
                }
                throw ArgumentParserError("cannot convert value to bool: " + value);
            }
            else if constexpr (std::is_integral<T>::value)
            {
                std::istringstream iss(value);
                T parsed{};
                iss >> parsed;
                if (iss.fail() || !iss.eof())
                {
                    throw ArgumentParserError("cannot convert value to integer: " + value);
                }
                return parsed;
            }
            else if constexpr (std::is_floating_point<T>::value)
            {
                std::istringstream iss(value);
                T parsed{};
                iss >> parsed;
                if (iss.fail() || !iss.eof())
                {
                    throw ArgumentParserError("cannot convert value to number: " + value);
                }
                return parsed;
            }
            else
            {
                static_assert(!sizeof(T *), "Unsupported conversion type");
            }
        }

    private:
        std::unordered_map<std::string, std::vector<std::string>> m_Data;
        std::unordered_map<std::string, bool> m_Provided;

        friend class ArgumentParser;
    };

    class ArgumentParser
    {
    public:
        class Argument
        {
        public:
            Argument &help(const std::string &text)
            {
                m_Help = text;
                return *this;
            }

            Argument &required(bool value = true)
            {
                m_Required = value;
                return *this;
            }

            Argument &metavar(const std::string &text)
            {
                m_Metavar = text;
                return *this;
            }

            Argument &action(const std::string &name)
            {
                if (name == "store")
                {
                    m_Action = ActionType::Store;
                    m_NargsMin = 1;
                    m_NargsMax = 1;
                }
                else if (name == "store_true")
                {
                    m_Action = ActionType::StoreTrue;
                    m_NargsMin = 0;
                    m_NargsMax = 0;
                }
                else if (name == "store_false")
                {
                    m_Action = ActionType::StoreFalse;
                    m_NargsMin = 0;
                    m_NargsMax = 0;
                }
                else if (name == "append")
                {
                    m_Action = ActionType::Append;
                    m_NargsMin = 1;
                    m_NargsMax = 1;
                }
                else if (name == "count")
                {
                    m_Action = ActionType::Count;
                    m_NargsMin = 0;
                    m_NargsMax = 0;
                }
                else if (name == "help")
                {
                    m_Action = ActionType::Help;
                    m_NargsMin = 0;
                    m_NargsMax = 0;
                }
                else
                {
                    throw ArgumentParserError("unknown action: " + name);
                }
                return *this;
            }

            Argument &nargs(std::size_t count)
            {
                m_NargsMin = count;
                m_NargsMax = count;
                return *this;
            }

            Argument &nargs(char pattern)
            {
                if (pattern == '?')
                {
                    m_NargsMin = 0;
                    m_NargsMax = 1;
                }
                else if (pattern == '*')
                {
                    m_NargsMin = 0;
                    m_NargsMax = std::numeric_limits<std::size_t>::max();
                }
                else if (pattern == '+')
                {
                    m_NargsMin = 1;
                    m_NargsMax = std::numeric_limits<std::size_t>::max();
                }
                else
                {
                    throw ArgumentParserError("unknown nargs pattern");
                }
                return *this;
            }

            template <typename T>
            Argument &default_value(T value)
            {
                m_DefaultValues = {to_string(value)};
                return *this;
            }

            Argument &default_values(std::initializer_list<std::string> values)
            {
                m_DefaultValues.assign(values.begin(), values.end());
                return *this;
            }

            template <typename T>
            Argument &implicit_value(T value)
            {
                m_ImplicitValue = to_string(value);
                return *this;
            }

            Argument &choices(std::initializer_list<std::string> values)
            {
                m_Choices.assign(values.begin(), values.end());
                return *this;
            }

            const std::string &dest() const
            {
                return m_Dest;
            }

        private:
            enum class Kind
            {
                Positional,
                Optional
            };

            enum class ActionType
            {
                Store,
                StoreTrue,
                StoreFalse,
                Append,
                Count,
                Help
            };

            static std::string sanitize_dest(const std::string &name)
            {
                std::string out;
                out.reserve(name.size());
                for (char c : name)
                {
                    if (c == '-')
                    {
                        out.push_back('_');
                    }
                    else
                    {
                        out.push_back(c);
                    }
                }
                return out;
            }

            template <typename T>
            static std::string to_string(const T &value)
            {
                if constexpr (std::is_same<T, std::string>::value)
                {
                    return value;
                }
                else if constexpr (std::is_same<T, const char *>::value)
                {
                    return std::string(value);
                }
                else if constexpr (std::is_same<T, bool>::value)
                {
                    return value ? "true" : "false";
                }
                else
                {
                    std::ostringstream oss;
                    oss << value;
                    return oss.str();
                }
            }

            bool takes_value() const
            {
                return m_NargsMax > 0;
            }

            bool is_unbounded() const
            {
                return m_NargsMax == std::numeric_limits<std::size_t>::max();
            }

        private:
            Kind m_Kind{Kind::Positional};
            ActionType m_Action{ActionType::Store};

            std::vector<std::string> m_OptionNames;
            std::string m_Dest;
            std::string m_Help;
            std::string m_Metavar;

            std::size_t m_NargsMin{1};
            std::size_t m_NargsMax{1};

            bool m_Required{false};

            std::vector<std::string> m_DefaultValues;
            std::optional<std::string> m_ImplicitValue;
            std::vector<std::string> m_Choices;

            std::vector<std::string> m_Values;
            std::size_t m_Occurrences{0};

            friend class ArgumentParser;
        };

    public:
        explicit ArgumentParser(std::string program = "", std::string description = "", std::string epilog = "")
            : m_Program(std::move(program)),
              m_Description(std::move(description)),
              m_Epilog(std::move(epilog))
        {
            if (m_AddHelp)
            {
                add_argument("-h", "--help")
                    .action("help")
                    .default_value(false)
                    .help("show this help message and exit");
            }
        }

        template <typename... Names>
        Argument &add_argument(const Names &...names)
        {
            std::vector<std::string> values{names...};
            if (values.empty())
            {
                throw ArgumentParserError("add_argument requires at least one name");
            }

            Argument arg;
            const bool optional = values.front().size() > 0 && values.front()[0] == '-';

            if (optional)
            {
                arg.m_Kind = Argument::Kind::Optional;
                arg.m_OptionNames = values;
                for (const auto &name : values)
                {
                    if (name.empty() || name[0] != '-')
                    {
                        throw ArgumentParserError("optional arguments must start with '-': " + name);
                    }
                }

                std::string dest_candidate;
                for (const auto &name : values)
                {
                    if (name.rfind("--", 0) == 0)
                    {
                        dest_candidate = name.substr(2);
                    }
                }
                if (dest_candidate.empty())
                {
                    dest_candidate = values.front().substr(1);
                }
                arg.m_Dest = Argument::sanitize_dest(dest_candidate);

                for (const auto &name : arg.m_OptionNames)
                {
                    if (m_OptionLookup.find(name) != m_OptionLookup.end())
                    {
                        throw ArgumentParserError("duplicate option: " + name);
                    }
                }
            }
            else
            {
                if (values.size() != 1)
                {
                    throw ArgumentParserError("positional arguments only accept one name");
                }
                arg.m_Kind = Argument::Kind::Positional;
                arg.m_Dest = values.front();
                arg.m_Required = true;
            }

            m_Arguments.push_back(std::move(arg));
            const auto idx = m_Arguments.size() - 1;

            if (m_Arguments[idx].m_Kind == Argument::Kind::Optional)
            {
                for (const auto &name : m_Arguments[idx].m_OptionNames)
                {
                    m_OptionLookup[name] = idx;
                }
            }
            else
            {
                m_Positionals.push_back(idx);
            }

            return m_Arguments[idx];
        }

        const Namespace &parse_args(int argc, char **argv)
        {
            if (m_Program.empty() && argc > 0 && argv != nullptr)
            {
                m_Program = argv[0];
            }

            for (auto &arg : m_Arguments)
            {
                arg.m_Values.clear();
                arg.m_Occurrences = 0;
            }

            std::vector<std::string> positional_tokens;

            int i = 1;
            while (i < argc)
            {
                const std::string token = argv[i];
                if (token == "--")
                {
                    ++i;
                    while (i < argc)
                    {
                        positional_tokens.emplace_back(argv[i]);
                        ++i;
                    }
                    break;
                }

                if (is_optional_token(token))
                {
                    if (token.rfind("--", 0) == 0)
                    {
                        consume_long_option(token, i, argc, argv);
                    }
                    else
                    {
                        consume_short_cluster(token, i, argc, argv);
                    }
                }
                else
                {
                    positional_tokens.emplace_back(token);
                }
                ++i;
            }

            assign_positionals(positional_tokens);
            finalize_and_validate();
            build_namespace();

            return m_Namespace;
        }

        std::string format_help() const
        {
            std::ostringstream out;
            out << "Usage: " << (m_Program.empty() ? "program" : m_Program);

            for (const auto &arg : m_Arguments)
            {
                if (arg.m_Kind == Argument::Kind::Optional)
                {
                    std::string head = arg.m_OptionNames.empty() ? arg.m_Dest : arg.m_OptionNames.back();
                    if (arg.takes_value())
                    {
                        const std::string mv = arg.m_Metavar.empty() ? to_upper(arg.m_Dest) : arg.m_Metavar;
                        if (arg.m_NargsMin == 0 && arg.m_NargsMax == 1)
                        {
                            out << " [" << head << " [" << mv << "]" << "]";
                        }
                        else
                        {
                            out << " [" << head << " " << mv;
                            if (arg.is_unbounded())
                            {
                                out << " ...";
                            }
                            out << "]";
                        }
                    }
                    else
                    {
                        out << " [" << head << "]";
                    }
                }
            }

            for (const auto idx : m_Positionals)
            {
                const auto &arg = m_Arguments[idx];
                const std::string mv = arg.m_Metavar.empty() ? arg.m_Dest : arg.m_Metavar;
                if (arg.m_NargsMin == 0 && arg.m_NargsMax == 1)
                {
                    out << " [" << mv << "]";
                }
                else if (arg.m_NargsMin == 0 && arg.is_unbounded())
                {
                    out << " [" << mv << " ...]";
                }
                else if (arg.m_NargsMin == 1 && arg.is_unbounded())
                {
                    out << " " << mv << " [" << mv << " ...]";
                }
                else
                {
                    for (std::size_t n = 0; n < arg.m_NargsMin; ++n)
                    {
                        out << " " << mv;
                    }
                }
            }

            out << '\n';

            if (!m_Description.empty())
            {
                out << '\n'
                    << m_Description << '\n';
            }

            out << '\n'
                << "Arguments:" << '\n';
            for (const auto &arg : m_Arguments)
            {
                out << "  " << argument_display(arg);
                if (!arg.m_Help.empty())
                {
                    out << "\t" << arg.m_Help;
                }
                if (!arg.m_DefaultValues.empty())
                {
                    out << " (default: " << arg.m_DefaultValues.front() << ")";
                }
                out << '\n';
            }

            if (!m_Epilog.empty())
            {
                out << '\n'
                    << m_Epilog << '\n';
            }

            return out.str();
        }

        void print_help(std::ostream &stream = std::cout) const
        {
            stream << format_help();
        }

    private:
        static bool is_optional_token(const std::string &token)
        {
            return token.size() > 1 && token[0] == '-';
        }

        static std::string to_upper(std::string text)
        {
            std::transform(text.begin(), text.end(), text.begin(),
                           [](unsigned char c)
                           { return static_cast<char>(std::toupper(c)); });
            return text;
        }

        static std::string argument_display(const Argument &arg)
        {
            if (arg.m_Kind == Argument::Kind::Positional)
            {
                return arg.m_Dest;
            }

            std::ostringstream oss;
            for (std::size_t i = 0; i < arg.m_OptionNames.size(); ++i)
            {
                if (i != 0)
                {
                    oss << ", ";
                }
                oss << arg.m_OptionNames[i];
            }

            if (arg.takes_value())
            {
                const std::string mv = arg.m_Metavar.empty() ? to_upper(arg.m_Dest) : arg.m_Metavar;
                oss << " " << mv;
            }
            return oss.str();
        }

        Argument &lookup_option(const std::string &name)
        {
            const auto it = m_OptionLookup.find(name);
            if (it == m_OptionLookup.end())
            {
                throw ArgumentParserError("unrecognized argument: " + name);
            }
            return m_Arguments[it->second];
        }

        void apply_action(Argument &arg, const std::vector<std::string> &values)
        {
            arg.m_Occurrences += 1;

            switch (arg.m_Action)
            {
            case Argument::ActionType::Help:
                throw HelpRequested();
            case Argument::ActionType::StoreTrue:
                arg.m_Values = {"true"};
                return;
            case Argument::ActionType::StoreFalse:
                arg.m_Values = {"false"};
                return;
            case Argument::ActionType::Count:
                arg.m_Values = {std::to_string(arg.m_Occurrences)};
                return;
            case Argument::ActionType::Append:
                for (const auto &v : values)
                {
                    arg.m_Values.emplace_back(v);
                }
                return;
            case Argument::ActionType::Store:
                arg.m_Values = values;
                return;
            }
        }

        std::vector<std::string> collect_option_values(Argument &arg,
                                                       int &index,
                                                       int argc,
                                                       char **argv,
                                                       std::optional<std::string> first)
        {
            std::vector<std::string> values;

            if (!arg.takes_value())
            {
                return values;
            }

            if (first.has_value())
            {
                values.push_back(*first);
            }

            while (values.size() < arg.m_NargsMin)
            {
                if (index + 1 >= argc)
                {
                    throw ArgumentParserError("argument requires a value: " + arg.m_Dest);
                }
                ++index;
                values.emplace_back(argv[index]);
            }

            while (arg.is_unbounded() && index + 1 < argc)
            {
                const std::string next = argv[index + 1];
                if (is_optional_token(next))
                {
                    break;
                }
                ++index;
                values.emplace_back(argv[index]);
            }

            while (!arg.is_unbounded() && values.size() < arg.m_NargsMax)
            {
                if (index + 1 >= argc)
                {
                    break;
                }
                const std::string next = argv[index + 1];
                if (is_optional_token(next))
                {
                    break;
                }
                ++index;
                values.emplace_back(argv[index]);
            }

            if (values.empty() && arg.m_ImplicitValue.has_value())
            {
                values.push_back(*arg.m_ImplicitValue);
            }

            if (values.size() < arg.m_NargsMin)
            {
                throw ArgumentParserError("argument requires more values: " + arg.m_Dest);
            }

            if (!arg.is_unbounded() && values.size() > arg.m_NargsMax)
            {
                values.resize(arg.m_NargsMax);
            }

            return values;
        }

        void consume_long_option(const std::string &token, int &index, int argc, char **argv)
        {
            const auto eq = token.find('=');
            const std::string name = (eq == std::string::npos) ? token : token.substr(0, eq);
            const std::optional<std::string> inline_value = (eq == std::string::npos)
                                                                ? std::nullopt
                                                                : std::optional<std::string>(token.substr(eq + 1));

            auto &arg = lookup_option(name);
            const auto values = collect_option_values(arg, index, argc, argv, inline_value);
            apply_action(arg, values);
        }

        void consume_short_cluster(const std::string &token, int &index, int argc, char **argv)
        {
            for (std::size_t pos = 1; pos < token.size(); ++pos)
            {
                const std::string name = std::string("-") + token[pos];
                auto &arg = lookup_option(name);

                std::optional<std::string> inline_value;
                if (arg.takes_value() && pos + 1 < token.size())
                {
                    inline_value = token.substr(pos + 1);
                    pos = token.size();
                }

                const auto values = collect_option_values(arg, index, argc, argv, inline_value);
                apply_action(arg, values);

                if (arg.takes_value())
                {
                    break;
                }
            }
        }

        void assign_positionals(const std::vector<std::string> &tokens)
        {
            std::size_t cursor = 0;
            for (std::size_t p = 0; p < m_Positionals.size(); ++p)
            {
                auto &arg = m_Arguments[m_Positionals[p]];
                const std::size_t remaining = tokens.size() - cursor;

                std::size_t remaining_minimum = 0;
                for (std::size_t j = p + 1; j < m_Positionals.size(); ++j)
                {
                    remaining_minimum += m_Arguments[m_Positionals[j]].m_NargsMin;
                }

                if (remaining < remaining_minimum + arg.m_NargsMin)
                {
                    throw ArgumentParserError("missing positional argument: " + arg.m_Dest);
                }

                std::size_t max_take = remaining - remaining_minimum;
                if (!arg.is_unbounded())
                {
                    max_take = std::min(max_take, arg.m_NargsMax);
                }

                std::size_t take = max_take;
                if (take < arg.m_NargsMin)
                {
                    throw ArgumentParserError("missing positional argument: " + arg.m_Dest);
                }

                for (std::size_t n = 0; n < take; ++n)
                {
                    arg.m_Values.emplace_back(tokens[cursor + n]);
                }
                cursor += take;
            }

            if (cursor < tokens.size())
            {
                throw ArgumentParserError("unrecognized positional argument: " + tokens[cursor]);
            }
        }

        void finalize_and_validate()
        {
            for (auto &arg : m_Arguments)
            {
                if (arg.m_Values.empty())
                {
                    if (!arg.m_DefaultValues.empty())
                    {
                        arg.m_Values = arg.m_DefaultValues;
                    }
                    else if (arg.m_Action == Argument::ActionType::StoreTrue)
                    {
                        arg.m_Values = {"false"};
                    }
                    else if (arg.m_Action == Argument::ActionType::StoreFalse)
                    {
                        arg.m_Values = {"true"};
                    }
                    else if (arg.m_Action == Argument::ActionType::Count)
                    {
                        arg.m_Values = {"0"};
                    }
                    else if (arg.m_ImplicitValue.has_value() && arg.m_NargsMin == 0)
                    {
                        arg.m_Values = {*arg.m_ImplicitValue};
                    }
                }

                if (arg.m_Required && arg.m_Values.empty() && arg.m_DefaultValues.empty())
                {
                    throw ArgumentParserError("required argument missing: " + arg.m_Dest);
                }

                if (!arg.m_Choices.empty())
                {
                    for (const auto &value : arg.m_Values)
                    {
                        if (std::find(arg.m_Choices.begin(), arg.m_Choices.end(), value) == arg.m_Choices.end())
                        {
                            throw ArgumentParserError("invalid choice for argument '" + arg.m_Dest + "': " + value);
                        }
                    }
                }
            }
        }

        void build_namespace()
        {
            m_Namespace.m_Data.clear();
            m_Namespace.m_Provided.clear();

            for (const auto &arg : m_Arguments)
            {
                m_Namespace.m_Data[arg.m_Dest] = arg.m_Values;
                m_Namespace.m_Provided[arg.m_Dest] = arg.m_Occurrences > 0;
            }
        }

    private:
        std::string m_Program;
        std::string m_Description;
        std::string m_Epilog;

        bool m_AddHelp{true};

        std::vector<Argument> m_Arguments;
        std::vector<std::size_t> m_Positionals;
        std::unordered_map<std::string, std::size_t> m_OptionLookup;

        Namespace m_Namespace;
    };

} // namespace argparser
