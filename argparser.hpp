/**
 * @file argparser.hpp
 * @brief Modern Argument Parser for C/C++.
 * @details All public API, types, macros, and configuration.
 * @author Sackey Ezekiel Etrue (djoezeke)
 * @version 0.1.0
 * @see https://www.github.com/djoezeke/argparser
 * @copyright Copyright (c) 2025 Sackey Ezekiel Etrue
 *
 * Developed by Sackey Ezekiel Etrue and every direct or indirect contributors.
 * See LICENSE for copyright and licensing details (standard MIT License).
 *
 * ARGPARSER: What is `argparser` ?
 *
 *      argparser is a modern C/C++ library for Commandline Argument Parsing.
 *
 *      MISSION:
 *
 *          - Easy to hack and improve.
 *          - Minimize setup and maintenance.
 *          - Efficient runtime and memory consumption.
 *
 *
 *          Designed primarily for developers and not the typical end-user!
 *
 *      DESIGN: Design Goals
 *
 *          PERFORMANCE FOCUSED :
 *              - Minimize memory allocations and copies.
 *              - Optimize for speed and low latency.
 *
 *          WELL DOCUMENTED :
 *              - Comprehensive API documentation.
 *              - Code comments and examples.
 *
 *          HEAVILY TESTED :
 *              - Unit tests for all features.
 *              - Integration tests for all components.
 *              - Performance tests for critical paths.
 *
 *          PORTABLE :
 *              - Cross-platform compatibility.
 *              - Support for various compilers and platforms.
 *
 *      FEATURES:
 *
 *          Brief overview of the features provided:
 *
 *          GENERAL :
 *
 *          STANDARD :
 *
 * NOTES:
 *
 * USAGE:
 *
 * FAQS:
 *
 * HELP:
 *    - See links below.
 *    - Read top of argparser.cpp for more details and comments.
 *
 *  Has only had a few tests run, may have issues.
 *
 *  If having issues compiling/linking/running raise an issue (https://github.com/djoezeke/argparser/issues).
 *  Please post in https://github.com/djoezeke/argparser/discussions if you cannot find a solution.
 *
 * RESOURCES:
 *
 * - Homepage ................... https://github.com/djoezeke/argparser
 * - Releases & changelog ....... https://github.com/djoezeke/argparser/releases
 * - Issues & support ........... https://github.com/djoezeke/argparser/issues
 *
 * LICENSE: MIT License
 *      See end of file for license information.
 *
 */

#ifndef DJOEZEKE_ARGPARSER_HPP

/**
 * SECTIONS: Index of this file
 *
 *  [SECTION] Include Mess
 *  [SECTION] Configurations
 *
 */

#define DJOEZEKE_ARGPARSER_HPP

// clang-format off

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
    #define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef ARGPARSER_SKIP_VERSION_CHECK
    #if defined(ARGPARSER_VERSION_MAJOR) && defined(ARGPARSER_VERSION_MINOR) && defined(ARGPARSER_VERSION_PATCH)
        #if ARGPARSER_VERSION_MAJOR != 1 || ARGPARSER_VERSION_MINOR != 0 || ARGPARSER_VERSION_PATCH != 0
            #warning "Already included a different version of the library!"
        #endif
    #endif
#endif // ARGPARSER_SKIP_VERSION_CHECK

/**
 * @defgroup version version Information
 * @brief Macros for library versioning.
 * @{
 */

/**
 * @def ARGPARSER_VERSION_MAJOR
 * @brief Major version number of the library.
 * @note If this were version 1.2.3, this value would be 1.
 * @since This macro is available since 0.1.0 .
 */
#ifndef ARGPARSER_VERSION_MAJOR
    #define ARGPARSER_VERSION_MAJOR 0
#endif // ARGPARSER_VERSION_MAJOR

/**
 * @def ARGPARSER_VERSION_MINOR
 * @brief Minor version number of the library.
 * @note If this were version 1.2.3, this value would be 2.
 * @since This macro is available since 0.1.0 .
 */
#ifndef ARGPARSER_VERSION_MINOR
    #define ARGPARSER_VERSION_MINOR 1
#endif // ARGPARSER_VERSION_MINOR

/**
 * @def ARGPARSER_VERSION_PATCH
 * @brief Patch version number of the library.
 * @note If this were version 1.2.3, this value would be 3.
 * @since This macro is available since 0.1.0 .
 */
#ifndef ARGPARSER_VERSION_PATCH
    #define ARGPARSER_VERSION_PATCH 0
#endif // ARGPARSER_VERSION_PATCH

/**
 * @def ARGPARSER_VERSION
 * @brief Library version string in the format @c "X.Y.Z",
 * where @c X is the major version number, @c Y is a minor version
 * number, and @c Z is the patch version number.
 * @sa MyGetVersion
 */
#ifndef ARGPARSER_VERSION
    #define ARGPARSER_VERSION "0.1.0"
#endif // ARGPARSER_VERSION

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] Include Mess
//-----------------------------------------------------------------------------

#include <limits>
#include <cctype>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <utility>
#include <iostream>
#include <optional>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <initializer_list>

//-----------------------------------------------------------------------------
// [SECTION] Configurations
//-----------------------------------------------------------------------------

/**
 * @defgroup configuration Library Configurations.
 * @brief Preprocessor macros for configuring library functionality.
 * @{
 */

/**
 * @brief Configure file with user config.
 */

#define BOLD 0
#define NORMAL 1
#define INTENSE 2
#define BACKGROUND 3
#define INTENSE_BACKGROUND 4

 #ifdef ARGPARSER_CONFIG
    #include ARGPARSER_CONFIG
#endif // ARGPARSER_CONFIG

#ifndef ARGPARSER_CONSOLE_WIDTH
    #define ARGPARSER_CONSOLE_WIDTH 80
#endif

/** @} */

// clang-format on

namespace argparser
{

    namespace detail
    {
        namespace format
        {

            class Colors
            {
                std::string reset = "\x1b[0m";

                std::vector<std::string> red = {"\x1b[31m", "", ""};
                std::vector<std::string> blue = {"\x1b[34m", "", ""};
                std::vector<std::string> cyan = {"\x1b[36m", "", ""};
                std::vector<std::string> green = {"\x1b[32m", "", ""};
                std::vector<std::string> black = {"\x1b[30m", "\x1b[1;30m", "\x1b[90m", "\x1b[40m", "\x1b[100m"};
                std::vector<std::string> white = {"\x1b[37m", "", ""};
                std::vector<std::string> yellow = {"\x1b[33m", "", ""};
                std::vector<std::string> magenta = {"\x1b[35m", "", ""};
            };

            bool isWhitespace(char c)
            {
                static std::string chars = " \t\n\r";
                return chars.find(c) != std::string::npos;
            }

            bool isBreakableBefore(char c)
            {
                static std::string chars = "[({<|";
                return chars.find(c) != std::string::npos;
            }

            bool isBreakableAfter(char c)
            {
                static std::string chars = "])}>.,:;*+-=&/\\";
                return chars.find(c) != std::string::npos;
            }

            class Columns;

            class Column
            {
                std::vector<std::string> m_strings;
                size_t m_width = ARGPARSER_CONSOLE_WIDTH;
                size_t m_indent = 0;
                size_t m_initialIndent = std::string::npos;

            public:
                class iterator
                {
                    friend Column;

                    Column const &m_column;
                    size_t m_stringIndex = 0;
                    size_t m_pos = 0;

                    size_t m_len = 0;
                    size_t m_end = 0;
                    bool m_suffix = false;

                    iterator(Column const &column, size_t stringIndex)
                        : m_column(column),
                          m_stringIndex(stringIndex)
                    {
                    }

                    auto line() const -> std::string const & { return m_column.m_strings[m_stringIndex]; }

                    auto isBoundary(size_t at) const -> bool
                    {
                        assert(at > 0);
                        assert(at <= line().size());

                        return at == line().size() ||
                               (isWhitespace(line()[at]) && !isWhitespace(line()[at - 1])) ||
                               isBreakableBefore(line()[at]) ||
                               isBreakableAfter(line()[at - 1]);
                    }

                    void calcLength()
                    {
                        assert(m_stringIndex < m_column.m_strings.size());

                        m_suffix = false;
                        auto width = m_column.m_width - indent();
                        m_end = m_pos;
                        if (line()[m_pos] == '\n')
                        {
                            ++m_end;
                        }
                        while (m_end < line().size() && line()[m_end] != '\n')
                            ++m_end;

                        if (m_end < m_pos + width)
                        {
                            m_len = m_end - m_pos;
                        }
                        else
                        {
                            size_t len = width;
                            while (len > 0 && !isBoundary(m_pos + len))
                                --len;
                            while (len > 0 && isWhitespace(line()[m_pos + len - 1]))
                                --len;

                            if (len > 0)
                            {
                                m_len = len;
                            }
                            else
                            {
                                m_suffix = true;
                                m_len = width - 1;
                            }
                        }
                    }

                    auto indent() const -> size_t
                    {
                        auto initial = m_pos == 0 && m_stringIndex == 0 ? m_column.m_initialIndent : std::string::npos;
                        return initial == std::string::npos ? m_column.m_indent : initial;
                    }

                    auto addIndentAndSuffix(std::string const &plain) const -> std::string
                    {
                        return std::string(indent(), ' ') + (m_suffix ? plain + "-" : plain);
                    }

                public:
                    using difference_type = std::ptrdiff_t;
                    using value_type = std::string;
                    using pointer = value_type *;
                    using reference = value_type &;
                    using iterator_category = std::forward_iterator_tag;

                    explicit iterator(Column const &column) : m_column(column)
                    {
                        assert(m_column.m_width > m_column.m_indent);
                        assert(m_column.m_initialIndent == std::string::npos || m_column.m_width > m_column.m_initialIndent);
                        calcLength();
                        if (m_len == 0)
                            m_stringIndex++; // Empty string
                    }

                    auto operator*() const -> std::string
                    {
                        assert(m_stringIndex < m_column.m_strings.size());
                        assert(m_pos <= m_end);
                        return addIndentAndSuffix(line().substr(m_pos, m_len));
                    }

                    auto operator++() -> iterator &
                    {
                        m_pos += m_len;
                        if (m_pos < line().size() && line()[m_pos] == '\n')
                            m_pos += 1;
                        else
                            while (m_pos < line().size() && isWhitespace(line()[m_pos]))
                                ++m_pos;

                        if (m_pos == line().size())
                        {
                            m_pos = 0;
                            ++m_stringIndex;
                        }
                        if (m_stringIndex < m_column.m_strings.size())
                            calcLength();
                        return *this;
                    }

                    auto operator++(int) -> iterator
                    {
                        iterator prev(*this);
                        operator++();
                        return prev;
                    }

                    auto operator==(iterator const &other) const -> bool
                    {
                        return m_pos == other.m_pos &&
                               m_stringIndex == other.m_stringIndex &&
                               &m_column == &other.m_column;
                    }
                    auto operator!=(iterator const &other) const -> bool
                    {
                        return !operator==(other);
                    }
                };

                using const_iterator = iterator;

                explicit Column(std::string const &text) { m_strings.push_back(text); }

                auto width(size_t newWidth) -> Column &
                {
                    assert(newWidth > 0);
                    m_width = newWidth;
                    return *this;
                }
                auto indent(size_t newIndent) -> Column &
                {
                    m_indent = newIndent;
                    return *this;
                }
                auto initialIndent(size_t newIndent) -> Column &
                {
                    m_initialIndent = newIndent;
                    return *this;
                }

                auto width() const -> size_t { return m_width; }
                auto begin() const -> iterator { return iterator(*this); }
                auto end() const -> iterator { return {*this, m_strings.size()}; }

                inline friend std::ostream &operator<<(std::ostream &os, Column const &col)
                {
                    bool first = true;
                    for (auto line : col)
                    {
                        if (first)
                            first = false;
                        else
                            os << "\n";
                        os << line;
                    }
                    return os;
                }

                auto operator+(Column const &other) -> Columns;

                auto toString() const -> std::string
                {
                    std::ostringstream oss;
                    oss << *this;
                    return oss.str();
                }
            };

            class Spacer : public Column
            {

            public:
                explicit Spacer(size_t spaceWidth) : Column("")
                {
                    width(spaceWidth);
                }
            };

            class Columns
            {
                std::vector<Column> m_columns;

            public:
                class iterator
                {
                    friend Columns;
                    struct EndTag
                    {
                    };

                    std::vector<Column> const &m_columns;
                    std::vector<Column::iterator> m_iterators;
                    size_t m_activeIterators;

                    iterator(Columns const &columns, EndTag)
                        : m_columns(columns.m_columns),
                          m_activeIterators(0)
                    {
                        m_iterators.reserve(m_columns.size());

                        for (auto const &col : m_columns)
                            m_iterators.push_back(col.end());
                    }

                public:
                    using difference_type = std::ptrdiff_t;
                    using value_type = std::string;
                    using pointer = value_type *;
                    using reference = value_type &;
                    using iterator_category = std::forward_iterator_tag;

                    explicit iterator(Columns const &columns)
                        : m_columns(columns.m_columns),
                          m_activeIterators(m_columns.size())
                    {
                        m_iterators.reserve(m_columns.size());

                        for (auto const &col : m_columns)
                            m_iterators.push_back(col.begin());
                    }

                    auto operator==(iterator const &other) const -> bool
                    {
                        return m_iterators == other.m_iterators;
                    }
                    auto operator!=(iterator const &other) const -> bool
                    {
                        return m_iterators != other.m_iterators;
                    }
                    auto operator*() const -> std::string
                    {
                        std::string row, padding;

                        for (size_t i = 0; i < m_columns.size(); ++i)
                        {
                            auto width = m_columns[i].width();
                            if (m_iterators[i] != m_columns[i].end())
                            {
                                std::string col = *m_iterators[i];
                                row += padding + col;
                                if (col.size() < width)
                                    padding = std::string(width - col.size(), ' ');
                                else
                                    padding = "";
                            }
                            else
                            {
                                padding += std::string(width, ' ');
                            }
                        }
                        return row;
                    }
                    auto operator++() -> iterator &
                    {
                        for (size_t i = 0; i < m_columns.size(); ++i)
                        {
                            if (m_iterators[i] != m_columns[i].end())
                                ++m_iterators[i];
                        }
                        return *this;
                    }
                    auto operator++(int) -> iterator
                    {
                        iterator prev(*this);
                        operator++();
                        return prev;
                    }
                };
                using const_iterator = iterator;

                auto begin() const -> iterator { return iterator(*this); }
                auto end() const -> iterator { return {*this, iterator::EndTag()}; }

                auto operator+=(Column const &col) -> Columns &
                {
                    m_columns.push_back(col);
                    return *this;
                }
                auto operator+(Column const &col) -> Columns
                {
                    Columns combined = *this;
                    combined += col;
                    return combined;
                }

                inline friend std::ostream &operator<<(std::ostream &os, Columns const &cols)
                {

                    bool first = true;
                    for (auto line : cols)
                    {
                        if (first)
                            first = false;
                        else
                            os << "\n";
                        os << line;
                    }
                    return os;
                }

                auto toString() const -> std::string
                {
                    std::ostringstream oss;
                    oss << *this;
                    return oss.str();
                }
            };

            inline auto Column::operator+(Column const &other) -> Columns
            {
                Columns cols;
                cols += *this;
                cols += other;
                return cols;
            }
        }
    }

#ifndef ARGPARSER_NO_EXCEPTIONS

    class ArgumentError : public std::runtime_error
    {
    public:
        explicit ArgumentError(const std::string &message)
            : std::runtime_error(message) {};
    };

    class ParseError : public ArgumentError
    {
    public:
        ParseError(const std::string &message)
            : ArgumentError(message) {};
    };

    class UsageError : public ArgumentError
    {
    public:
        UsageError(const std::string &message)
            : ArgumentError(message) {};
    };

    class RequiredError : public ArgumentError
    {
    public:
        RequiredError(const std::string &message)
            : ArgumentError(message) {};
    };

    class HelpError : public ArgumentError
    {
    public:
        HelpError()
            : ArgumentError("help requested") {};
    };

#endif // ARGPARSER_NO_EXCEPTIONS

    class HelpFormatter
    {
    public:
        class Section
        {
            friend class HelpFormatter;

        private:
            std::vector<Section> m_Sections;
            HelpFormatter *m_Formatter;
            std::string m_Heading;
        };

    public:
        HelpFormatter(std::string program = "", int width = 0, int indent = 4)
        {
            m_Width = width;
            m_Indent = indent;
            m_Level = 0;
            m_CIndent = 0;
        };

        HelpFormatter &add_help() {};
        HelpFormatter &add_text(std::string text) {};
        HelpFormatter &add_usage(std::string usage, std::vector<std::string> args) {};

    private:
        std::string format_help() {};
        std::string format_text(std::string text) {};
        std::string format_usage(std::string usage, std::vector<std::string> args) {};

    private:
        void indent()
        {
            m_CIndent += m_Indent;
            m_Level += 1;
        };

        void dedent()
        {
            m_CIndent -= m_Indent;
            m_Level -= 1;
        };

    private:
        std::vector<Section> m_Sections;
        int m_Width;
        int m_Level;
        int m_Indent;
        int m_CIndent;
    };

    class Namespace
    {
        friend class ArgumentParser;

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

#ifndef ARGPARSER_NO_EXCEPTIONS
                throw ParseError("unknown argument: " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
            }
            return it->second;
        }

        template <typename T>
        T get(const std::string &name) const
        {
            const auto &vals = raw_values(name);
            if (vals.empty())
            {
#ifndef ARGPARSER_NO_EXCEPTIONS
                throw ArgumentError("argument has no value: " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
            }
            return convert<T>(vals.front());
        }

        template <typename T>
        std::vector<T> getlist(const std::string &name) const
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                throw ArgumentError("cannot convert value to bool: " + value);
#endif // ARGPARSER_NO_EXCEPTIONS
            }
            else if constexpr (std::is_integral<T>::value)
            {
                std::istringstream iss(value);
                T parsed{};
                iss >> parsed;
                if (iss.fail() || !iss.eof())
                {
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw ArgumentError("cannot convert value to integer: " + value);
#endif // ARGPARSER_NO_EXCEPTIONS
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw ArgumentError("cannot convert value to number: " + value);
#endif // ARGPARSER_NO_EXCEPTIONS
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
    };

    class ArgumentParser
    {
    public:
        class Argument
        {
            friend class ArgumentParser;

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
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw ArgumentError("unknown action: " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw ParseError("unknown nargs pattern");
#endif // ARGPARSER_NO_EXCEPTIONS
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
        };

    public:
        explicit ArgumentParser(std::string program = "", std::string description = "", std::string epilog = "")
            : m_Program(std::move(program)),
              m_Description(std::move(description)),
              m_Epilog(std::move(epilog)),
              m_HelpFormatter(HelpFormatter())
        {
            if (m_AddHelp)
            {
                add_argument("-h", "--help")
                    .action("help")
                    .default_value(false)
                    .help("show this help message and exit");
            }
        }

        ArgumentParser &program(std::string program)
        {
            m_Program = program;
            return *this;
        };

        ArgumentParser &usage(std::string usage)
        {
            m_Usage = usage;
            return *this;
        };

        ArgumentParser &epilog(std::string epilog)
        {
            m_Epilog = epilog;
            return *this;
        };

        ArgumentParser &formatter(HelpFormatter formatter)
        {
            m_HelpFormatter = formatter;
            return *this;
        };

        ArgumentParser &description(std::string description)
        {
            m_Description = description;
            return *this;
        };

        template <typename... Names>
        Argument &add_argument(const Names &...names)
        {
            std::vector<std::string> values{names...};
            if (values.empty())
            {
#ifndef ARGPARSER_NO_EXCEPTIONS
                throw ArgumentError("add_argument requires at least one name");
#endif // ARGPARSER_NO_EXCEPTIONS
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                        throw ArgumentError("optional arguments must start with '-': " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                        throw ArgumentError("duplicate option: " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
                    }
                }
            }
            else
            {
                if (values.size() != 1)
                {
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw ArgumentError("positional arguments only accept one name");
#endif // ARGPARSER_NO_EXCEPTIONS
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

        std::string format_usage() const
        {
            std::ostringstream out;
            out << "Usage: " << (m_Program.empty() ? "program" : m_Program);
            return out.str();
        }

        std::string format_help() const
        {
            std::ostringstream out;

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

        void print_usage(std::ostream &stream = std::cout) const
        {
            stream << format_usage();
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                throw ParseError("unrecognized argument: " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
            }
            return m_Arguments[it->second];
        }

        void apply_action(Argument &arg, const std::vector<std::string> &values)
        {
            arg.m_Occurrences += 1;

            switch (arg.m_Action)
            {
            case Argument::ActionType::Help:
#ifndef ARGPARSER_NO_EXCEPTIONS
                throw HelpError();
#endif // ARGPARSER_NO_EXCEPTIONS
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw RequiredError("argument requires a value: " + arg.m_Dest);
#endif // ARGPARSER_NO_EXCEPTIONS
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                throw RequiredError("argument requires more values: " + arg.m_Dest);
#endif // ARGPARSER_NO_EXCEPTIONS
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw RequiredError("missing positional argument: " + arg.m_Dest);
#endif // ARGPARSER_NO_EXCEPTIONS
                }

                std::size_t max_take = remaining - remaining_minimum;
                if (!arg.is_unbounded())
                {
                    max_take = std::min(max_take, arg.m_NargsMax);
                }

                std::size_t take = max_take;
                if (take < arg.m_NargsMin)
                {
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw RequiredError("missing positional argument: " + arg.m_Dest);
#endif // ARGPARSER_NO_EXCEPTIONS
                }

                for (std::size_t n = 0; n < take; ++n)
                {
                    arg.m_Values.emplace_back(tokens[cursor + n]);
                }
                cursor += take;
            }

            if (cursor < tokens.size())
            {
#ifndef ARGPARSER_NO_EXCEPTIONS
                throw ArgumentError("unrecognized positional argument: " + tokens[cursor]);
#endif // ARGPARSER_NO_EXCEPTIONS
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
#ifndef ARGPARSER_NO_EXCEPTIONS
                    throw RequiredError("required argument missing: " + arg.m_Dest);
#endif // ARGPARSER_NO_EXCEPTIONS
                }

                if (!arg.m_Choices.empty())
                {
                    for (const auto &value : arg.m_Values)
                    {
                        if (std::find(arg.m_Choices.begin(), arg.m_Choices.end(), value) == arg.m_Choices.end())
                        {
#ifndef ARGPARSER_NO_EXCEPTIONS
                            throw ArgumentError("invalid choice for argument '" + arg.m_Dest + "': " + value);
#endif // ARGPARSER_NO_EXCEPTIONS
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
        std::string m_Usage;
        std::string m_Epilog;
        std::string m_Program;
        std::string m_Description;

        bool m_AddHelp{true};

        std::vector<Argument> m_Arguments;
        std::vector<std::size_t> m_Positionals;
        std::unordered_map<std::string, std::size_t> m_OptionLookup;

        Namespace m_Namespace;
        HelpFormatter m_HelpFormatter;
    };

    using Argparser = ArgumentParser;

} // namespace argparser

#endif // DJOEZEKE_ARGPARSER_HPP

/**
 * LICENSE: MIT License
 *
 * Copyright (c) 2025 Sackey Ezekiel Etrue
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */