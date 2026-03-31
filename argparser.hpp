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
#include <cstdio>

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

 #ifdef ARGPARSER_CONFIG
    #include ARGPARSER_CONFIG
#endif // ARGPARSER_CONFIG

/** @} */

// clang-format on

namespace argparser
{

}

namespace argparser
{

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

    /**
     * @class HelpFormatter
     * @brief Advanced help message formatter with colors, sections, and extensive customization.
     *
     * Features:
     * - ANSI color codes and custom themes
     * - Automatic text wrapping with configurable width
     * - Logical sections for organizing arguments
     * - Custom usage patterns and epilog messages
     * - Multiple output formats (plain, colored, markdown-ready)
     * - Customizable indentation and alignment
     * - Support for examples, notes, and warnings sections
     * - Column width auto-detection and customization
     */
    class HelpFormatter
    {
    public:
        /**
         * @enum Color - ANSI color codes for terminal output
         */
        enum class Color
        {
            Reset = 0,
            Bold = 1,
            Dim = 2,
            Italic = 3,
            Underline = 4,
            Blink = 5,
            Reverse = 7,
            Hidden = 8,
            Black = 30,
            Red = 31,
            Green = 32,
            Yellow = 33,
            Blue = 34,
            Magenta = 35,
            Cyan = 36,
            White = 37,
            Default = 39,
            BgBlack = 40,
            BgRed = 41,
            BgGreen = 42,
            BgYellow = 43,
            BgBlue = 44,
            BgMagenta = 45,
            BgCyan = 46,
            BgWhite = 47,
            BgDefault = 49
        };

        /**
         * @enum Format - Different help output formats
         */
        enum class Format
        {
            Plain,    ///< Plain text without colors
            Colored,  ///< Text with ANSI colors
            Markdown, ///< Markdown format
            Compact   ///< Compact format (single-line descriptions)
        };

        /**
         * @struct ColorTheme - Color scheme for help messages
         */
        struct ColorTheme
        {
            bool enabled = true;
            Color usage_color = Color::Green;
            Color option_color = Color::Blue;
            Color warnings_color = Color::Red;
            Color required_color = Color::Red;
            Color heading_color = Color::Bold;
            Color section_color = Color::Cyan;
            Color notes_color = Color::Magenta;
            Color examples_color = Color::Green;
            Color metavar_color = Color::Yellow;
            Color description_color = Color::Default;
        };

        /**
         * @struct Alignment - Layout configuration
         */
        struct Alignment
        {
            size_t indent_size = 2;
            char indent_char = ' ';
            size_t option_width = 20;
            bool wrap_help_text = true;
            size_t max_help_position = 24;
            size_t min_description_width = 20;
        };

        /**
         * @class Section - Logical grouping of help content
         */
        class Section
        {
        public:
            enum class Type
            {
                Notes,     ///< Additional notes
                Custom,    ///< Custom section
                Examples,  ///< Usage examples
                Warnings,  ///< Warning messages
                Arguments, ///< Arguments section
            };

            Section(const std::string &title = "", const std::string &description = "",
                    Type type = Type::Custom);

            Section &add_item(const std::string &option, const std::string &metavar,
                              const std::string &help, bool required = false);

            Section &add_text(const std::string &text);
            Section &add_example(const std::string &command, const std::string &description);
            bool visible();
            Section &visible(bool visible);
            const std::string &title() const;
            const std::string &description() const;
            Type type() const;
            bool visible() const;

        private:
            friend class HelpFormatter;

            struct Item
            {
                std::string option;
                std::string metavar;
                std::string help;
                bool required;
            };

            struct Example
            {
                std::string command;
                std::string description;
            };

            std::string m_Title;
            std::string m_Description;
            Type m_Type;
            bool m_Visible = true;
            std::vector<Item> m_Items;
            std::vector<std::string> m_Texts;
            std::vector<Example> m_Examples;
        };

    public:
        /**
         * @brief Constructs HelpFormatter with configuration
         */
        HelpFormatter(size_t width = 80, const std::string &prog_name = "");
        HelpFormatter &program(const std::string &name);
        HelpFormatter &usage(const std::string &usage);
        HelpFormatter &epilog(const std::string &epilog);
        HelpFormatter &description(const std::string &description);

        /**
         * @brief Sets prefix information (displayed before usage)
         * @param info The prefix text
         * @return Reference to this helpformatter.
         */
        HelpFormatter &prefix_info(const std::string &info);

        /**
         * @brief Sets suffix information (displayed at the end)
         * @param info The suffix text
         * @return Reference to this helpformatter.
         */
        HelpFormatter &suffix_info(const std::string &info);

        /**
         * @brief Sets custom help text width
         * @param width Width in characters (0 = auto-detect)
         * @return Reference to this helpformatter.
         */
        HelpFormatter &width(size_t width);

        /**
         * @brief Customizes format for help text
         * @param format Output format
         * @return Reference to this helpformatter.
         */
        HelpFormatter &format(Format format);

        /**
         * @brief Customizes alignment settings for help text
         * @param alignment The alignment configuration
         * @return Reference to this helpformatter.
         */
        HelpFormatter &alignment(const Alignment &align);

        /**
         * @brief Enables or disables help text color output
         * @param enable True to enable colors
         * @return Reference to this helpformatter.
         */
        HelpFormatter &use_color(bool enable);

        /**
         * @brief Customizes the color theme for help messages
         * @param theme The color theme configuration
         * @return Reference to this helpformatter.
         */
        HelpFormatter &color_theme(const ColorTheme &theme);

        /**
         * @brief Sets a line separator in the help text
         * @param separator The separator string (e.g., "=" or "-")
         * @return Reference to this helpformatter.
         */
        HelpFormatter &line_separator(const std::string &separator);

        /**
         * @brief Sets a line separator in the help text
         * @param separator The separator string (e.g., "=" or "-")
         * @param width The width of the separator
         * @return Reference to this helpformatter.
         */
        HelpFormatter &line_separator(const char sep = '-', int width = 80);

        HelpFormatter &sort_sections(bool sort);

        // === Section Management ===

        Section &add_section(const std::string &title, const std::string &description = "",
                             Section::Type type = Section::Type::Custom);

        Section &get_section(const std::string &title, const std::string &description = "",
                             Section::Type type = Section::Type::Custom);

        // === Getter Methods ===

        const std::string &program() const;
        size_t width() const;
        ColorTheme &color_theme();
        const ColorTheme &color_theme() const;
        Alignment &alignment();
        const Alignment &alignment() const;
        const std::string &usage() const;
        const std::string &epilog() const;
        const std::string &description() const;

        // === Formatting Methods ===

        // std::string format_help(const std::vector<ArgumentParser::Argument> *arguments = nullptr) const
        std::string format_help() const;

        // std::string format_usage(const std::vector<ArgumentParser::Argument> *arguments = nullptr) const
        std::string format_usage() const;
        std::string format_section(const Section &section) const;
        std::string wrap_text(const std::string &text, size_t indent_level = 0) const;
        std::string colorize(const std::string &text, Color color) const;

    private:
        static size_t get_terminal_width();

        Color get_color_for_format(Color default_color) const;
        std::string format_colored(const std::string &text, Color color) const;
        std::string wrap_words(const std::vector<std::string> &words, size_t indent_level) const;
        std::string format_item(const Section::Item &item) const;
        std::string format_example(const Section::Example &example) const;

    private:
        size_t m_Width;
        std::string m_Program;
        std::string m_Usage;
        std::string m_Epilog;
        std::string m_Description;
        std::string m_PrefixInfo;
        std::string m_SuffixInfo;
        std::string m_LineSeparator;
        bool m_SortSections = false;
        std::vector<Section> m_Sections;
        ColorTheme m_Theme;
        Alignment m_Alignment;
        Format m_Format;
    };

    class Namespace
    {
        friend class ArgumentParser;

    public:
        bool has(const std::string &name) const;
        bool provided(const std::string &name) const;
        const std::vector<std::string> &raw_values(const std::string &name) const;
        template <typename T>
        T get(const std::string &name) const;
        template <typename T>
        std::vector<T> getlist(const std::string &name) const;

    private:
        template <typename T>
        static T convert(const std::string &value);

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

        public:
            Argument &help(const std::string &text);
            Argument &required(bool value = true);
            Argument &metavar(const std::string &text);
            Argument &action(const std::string &name);
            Argument &nargs(std::size_t count);
            Argument &nargs(char pattern);
            template <typename T>
            Argument &default_value(T value);
            Argument &default_values(std::initializer_list<std::string> values);
            template <typename T>
            Argument &implicit_value(T value);
            Argument &choices(std::initializer_list<std::string> values);
            const std::string &dest() const;

        private:
            static std::string sanitize_dest(const std::string &name);
            template <typename T>
            static std::string to_string(const T &value);
            bool takes_value() const;
            bool is_unbounded() const;

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
        ArgumentParser(std::string program = "", std::string description = "", std::string epilog = "");
        std::string &program();
        ArgumentParser &program(std::string program);
        std::string &usage();
        ArgumentParser &usage(std::string usage);
        std::string &epilog();
        ArgumentParser &epilog(std::string epilog);
        HelpFormatter &formatter();
        ArgumentParser &formatter(HelpFormatter formatter);
        std::string &description();
        ArgumentParser &description(std::string description);

        /**
         * @brief Format help message
         * @return The complete help text
         */
        std::string format_help() const;

        /**
         * @brief Format usage message
         * @return The complete usage text
         */
        std::string format_usage() const;

        /**
         * @brief Gets a const reference to the HelpFormatter
         * @return Const reference to the internal HelpFormatter
         */
        const HelpFormatter &get_formatter() const;

        template <typename... Names>
        Argument &add_argument(const Names &...names);

        const Namespace &parse_args(int argc, char **argv);

        /**
         * @brief Prints the help message
         * @param stream to write to (default=stdout).
         */
        void print_help(std::ostream &stream = std::cout) const;

        /**
         * @brief Prints the usage message
         * @param stream to write to (default=stdout).
         */
        void print_usage(std::ostream &stream = std::cout) const;

    private:
        static bool is_optional_token(const std::string &token);
        static std::string to_upper(std::string text);
        static std::string argument_display(const Argument &arg);
        Argument &lookup_option(const std::string &name);
        void apply_action(Argument &arg, const std::vector<std::string> &values);
        std::vector<std::string> collect_option_values(Argument &arg,
                                                       int &index,
                                                       int argc,
                                                       char **argv,
                                                       std::optional<std::string> first);

        void consume_long_option(const std::string &token, int &index, int argc, char **argv);
        void consume_short_cluster(const std::string &token, int &index, int argc, char **argv);
        void assign_positionals(const std::vector<std::string> &tokens);
        void finalize_and_validate();
        void build_namespace();

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
    using Formatter = HelpFormatter;
    using Section = HelpFormatter::Section;
    using Alignment = HelpFormatter::Alignment;
    using ColorTheme = HelpFormatter::ColorTheme;

} // namespace argparser

#endif // DJOEZEKE_ARGPARSER_HPP

#pragma region Argparser

#define ARGPARSER_IMPLEMENTATION
#ifdef ARGPARSER_IMPLEMENTATION

namespace argparser
{

    //-----------------------------------------------------------------------------
    // [Class] HelpFormatter
    //-----------------------------------------------------------------------------
    // - HelpFormatter()
    //-----------------------------------------------------------------------------

    HelpFormatter::Section::Section(const std::string &title, const std::string &description,
                                    Type type)
        : m_Title(title), m_Description(description), m_Type(type) {}

    HelpFormatter::Section &HelpFormatter::Section::add_item(const std::string &option, const std::string &metavar,
                                                             const std::string &help, bool required)
    {
        m_Items.push_back({option, metavar, help, required});
        return *this;
    }

    HelpFormatter::Section &HelpFormatter::Section::add_text(const std::string &text)
    {
        m_Texts.push_back(text);
        return *this;
    }

    HelpFormatter::Section &HelpFormatter::Section::add_example(const std::string &command, const std::string &description)
    {
        m_Examples.push_back({command, description});
        return *this;
    }

    bool HelpFormatter::Section::visible()
    {
        return m_Visible;
    }

    HelpFormatter::Section &HelpFormatter::Section::visible(bool visible)
    {
        m_Visible = visible;
        return *this;
    }

    const std::string &HelpFormatter::Section::title() const { return m_Title; }
    const std::string &HelpFormatter::Section::description() const { return m_Description; }
    HelpFormatter::Section::Type HelpFormatter::Section::type() const { return m_Type; }
    bool HelpFormatter::Section::visible() const { return m_Visible; }

    HelpFormatter::HelpFormatter(size_t width, const std::string &prog_name)
        : m_Width(width), m_Program(prog_name), m_Theme(ColorTheme()),
          m_Format(Format::Colored)
    {
        if (m_Width == 0)
            m_Width = get_terminal_width();
    }

    HelpFormatter &HelpFormatter::program(const std::string &name)
    {
        m_Program = name;
        return *this;
    }

    HelpFormatter &HelpFormatter::usage(const std::string &usage)
    {
        m_Usage = usage;
        return *this;
    }

    HelpFormatter &HelpFormatter::epilog(const std::string &epilog)
    {
        m_Epilog = epilog;
        return *this;
    }

    HelpFormatter &HelpFormatter::description(const std::string &description)
    {
        m_Description = description;
        return *this;
    }

    HelpFormatter &HelpFormatter::prefix_info(const std::string &info)
    {
        m_PrefixInfo = info;
        return *this;
    }

    HelpFormatter &HelpFormatter::suffix_info(const std::string &info)
    {
        m_SuffixInfo = info;
        return *this;
    }

    HelpFormatter &HelpFormatter::width(size_t width)
    {
        m_Width = width ? width : get_terminal_width();
        return *this;
    }

    HelpFormatter &HelpFormatter::format(Format format)
    {
        m_Format = format;
        return *this;
    }

    HelpFormatter &HelpFormatter::alignment(const Alignment &align)
    {
        m_Alignment = align;
        return *this;
    }

    HelpFormatter &HelpFormatter::use_color(bool enable)
    {
        m_Theme.enabled = enable;
        return *this;
    }

    HelpFormatter &HelpFormatter::color_theme(const ColorTheme &theme)
    {
        m_Theme = theme;
        return *this;
    }

    HelpFormatter &HelpFormatter::line_separator(const std::string &separator)
    {
        m_LineSeparator = separator;
        return *this;
    }

    HelpFormatter &HelpFormatter::line_separator(const char sep, int width)
    {
        line_separator(std::string(width, sep));
        return *this;
    }

    HelpFormatter &HelpFormatter::sort_sections(bool sort)
    {
        m_SortSections = sort;
        return *this;
    }

    HelpFormatter::Section &HelpFormatter::add_section(const std::string &title, const std::string &description,
                                                       Section::Type type)
    {
        m_Sections.emplace_back(title, description, type);
        return m_Sections.back();
    }

    HelpFormatter::Section &HelpFormatter::get_section(const std::string &title, const std::string &description,
                                                       Section::Type type)
    {
        for (auto &section : m_Sections)
        {
            if (section.title() == title)
                return section;
        }
        return add_section(title, description, type);
    }

    const std::string &HelpFormatter::program() const { return m_Program; }
    size_t HelpFormatter::width() const { return m_Width; }
    HelpFormatter::ColorTheme &HelpFormatter::color_theme() { return m_Theme; }
    const HelpFormatter::ColorTheme &HelpFormatter::color_theme() const { return m_Theme; }
    HelpFormatter::Alignment &HelpFormatter::alignment() { return m_Alignment; }
    const HelpFormatter::Alignment &HelpFormatter::alignment() const { return m_Alignment; }
    const std::string &HelpFormatter::usage() const { return m_Usage; }
    const std::string &HelpFormatter::epilog() const { return m_Epilog; }
    const std::string &HelpFormatter::description() const { return m_Description; }

    std::string HelpFormatter::format_help() const
    {
        std::ostringstream oss;

        if (!m_PrefixInfo.empty())
            oss << m_PrefixInfo << "\n";

        if (!m_Usage.empty())
        {
            oss << format_colored("usage: ", get_color_for_format(Color::Green))
                << m_Usage << "\n\n";
        }
        else if (!m_Program.empty())
        {
            oss << format_colored("usage: ", get_color_for_format(Color::Green))
                << m_Program << " [options] [arguments]\n\n";
        }

        if (!m_Description.empty())
            oss << wrap_text(m_Description, 0) << "\n\n";

        if (!m_LineSeparator.empty())
            oss << m_LineSeparator << "\n";

        for (const auto &section : m_Sections)
        {
            if (section.visible())
                oss << format_section(section);
        }

        if (!m_LineSeparator.empty() && !m_Epilog.empty())
            oss << m_LineSeparator << "\n";

        if (!m_Epilog.empty())
            oss << "\n"
                << wrap_text(m_Epilog, 0) << "\n";

        if (!m_SuffixInfo.empty())
            oss << "\n"
                << m_SuffixInfo << "\n";

        return oss.str();
    }

    std::string HelpFormatter::format_usage() const
    {
        std::ostringstream oss;
        oss << format_colored("usage: ", get_color_for_format(Color::Green)) << m_Program << " ";
        return oss.str();
    }

    std::string HelpFormatter::format_section(const Section &section) const
    {
        std::ostringstream oss;

        if (!section.title().empty())
        {
            Color heading = (m_Format == Format::Compact) ? Color::Default : m_Theme.heading_color;
            // oss << format_colored(section.title() + ":", heading) << "\n";
            oss << format_colored(section.title() + ":", heading);
        }

        if (!section.description().empty())
            oss << wrap_text(section.description(), m_Alignment.indent_size) << "\n\n";

        for (const auto &item : section.m_Items)
            oss << format_item(item) << "\n";

        for (const auto &example : section.m_Examples)
            oss << format_example(example) << "\n";

        for (const auto &text : section.m_Texts)
            oss << wrap_text(text, m_Alignment.indent_size) << "\n";

        oss << "\n";
        return oss.str();
    }

    std::string HelpFormatter::wrap_text(const std::string &text, size_t indent_level) const
    {
        if (!m_Alignment.wrap_help_text)
            return text;

        std::vector<std::string> words;
        std::istringstream iss(text);
        std::string word;
        while (iss >> word)
            words.push_back(word);
        return wrap_words(words, indent_level);
    }

    std::string HelpFormatter::colorize(const std::string &text, Color color) const
    {
        if (m_Format == Format::Plain || !m_Theme.enabled)
            return text;
        return format_colored(text, color);
    }

    size_t HelpFormatter::get_terminal_width()
    {
#ifdef _WIN32
        return 80;
#else
        FILE *fp = popen("tput cols 2>/dev/null", "r");
        if (fp)
        {
            int width = 0;
            if (fscanf(fp, "%d", &width) == 1)
            {
                pclose(fp);
                return width > 0 ? width : 80;
            }
            pclose(fp);
        }
        return 80;
#endif
    }

    HelpFormatter::Color HelpFormatter::get_color_for_format(Color default_color) const
    {
        return (m_Format == Format::Plain || !m_Theme.enabled) ? Color::Default : default_color;
    }

    std::string HelpFormatter::format_colored(const std::string &text, Color color) const
    {
        if (m_Format == Format::Plain || !m_Theme.enabled || color == Color::Default)
            return text;

        std::ostringstream oss;
        oss << "\033[" << static_cast<int>(color) << "m" << text << "\033[0m";
        return oss.str();
    }

    std::string HelpFormatter::wrap_words(const std::vector<std::string> &words, size_t indent_level) const
    {
        std::ostringstream oss;
        const std::string indent(indent_level * m_Alignment.indent_size, m_Alignment.indent_char);
        size_t line_length = indent_level * m_Alignment.indent_size;
        bool first_line = true;

        for (const auto &word : words)
        {
            size_t word_len = word.length();

            if (line_length + word_len + 1 <= m_Width)
            {
                if (!first_line && line_length > indent_level * m_Alignment.indent_size)
                {
                    oss << " ";
                    line_length++;
                }
                oss << word;
                line_length += word_len;
            }
            else
            {
                oss << "\n"
                    << indent << word;
                line_length = indent_level * m_Alignment.indent_size + word_len;
            }
            first_line = false;
        }

        return oss.str();
    }

    std::string HelpFormatter::format_item(const Section::Item &item) const
    {
        std::ostringstream oss;
        const std::string indent(m_Alignment.indent_size, m_Alignment.indent_char);

        oss << indent;

        std::string option_str = item.option;
        if (item.required)
            option_str += format_colored(" [REQUIRED]", m_Theme.required_color);

        oss << format_colored(option_str, m_Theme.option_color);

        if (!item.metavar.empty())
            oss << " " << format_colored(item.metavar, m_Theme.metavar_color);

        size_t current_pos = item.option.length();
        if (!item.metavar.empty())
            current_pos += item.metavar.length() + 1;
        current_pos += m_Alignment.indent_size;

        size_t help_start_pos = m_Alignment.max_help_position;
        if (current_pos < help_start_pos)
            oss << std::string(help_start_pos - current_pos, ' ');
        else
            oss << "  ";

        oss << wrap_text(item.help, help_start_pos / m_Alignment.indent_size);

        return oss.str();
    }

    std::string HelpFormatter::format_example(const Section::Example &example) const
    {
        std::ostringstream oss;
        const std::string indent(m_Alignment.indent_size, m_Alignment.indent_char);

        oss << indent << "  $ " << format_colored(example.command, m_Theme.examples_color);
        if (!example.description.empty())
            oss << "\n"
                << wrap_text(example.description, m_Alignment.indent_size + 2);

        return oss.str();
    }

    //-----------------------------------------------------------------------------
    // [Class] Namespace
    //-----------------------------------------------------------------------------
    // - Namespace()
    //-----------------------------------------------------------------------------

    bool Namespace::has(const std::string &name) const
    {
        return m_Data.find(name) != m_Data.end();
    }

    bool Namespace::provided(const std::string &name) const
    {
        const auto it = m_Provided.find(name);
        return it != m_Provided.end() && it->second;
    }

    const std::vector<std::string> &Namespace::raw_values(const std::string &name) const
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
    T Namespace::get(const std::string &name) const
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
    std::vector<T> Namespace::getlist(const std::string &name) const
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

    template <typename T>
    T Namespace::convert(const std::string &value)
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

    //-----------------------------------------------------------------------------
    // [Class] ArgumentParser
    //-----------------------------------------------------------------------------
    // - ArgumentParser()
    //-----------------------------------------------------------------------------

    ArgumentParser::Argument &ArgumentParser::Argument::help(const std::string &text)
    {
        m_Help = text;
        return *this;
    };

    ArgumentParser::Argument &ArgumentParser::Argument::required(bool value)
    {
        m_Required = value;
        return *this;
    }

    ArgumentParser::Argument &ArgumentParser::Argument::metavar(const std::string &text)
    {
        m_Metavar = text;
        return *this;
    }

    ArgumentParser::Argument &ArgumentParser::Argument::action(const std::string &name)
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

    ArgumentParser::Argument &ArgumentParser::Argument::nargs(std::size_t count)
    {
        m_NargsMin = count;
        m_NargsMax = count;
        return *this;
    }

    ArgumentParser::Argument &ArgumentParser::Argument::nargs(char pattern)
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
    ArgumentParser::Argument &ArgumentParser::Argument::default_value(T value)
    {
        m_DefaultValues = {to_string(value)};
        return *this;
    }

    ArgumentParser::Argument &ArgumentParser::Argument::default_values(std::initializer_list<std::string> values)
    {
        m_DefaultValues.assign(values.begin(), values.end());
        return *this;
    }

    template <typename T>
    ArgumentParser::Argument &ArgumentParser::Argument::implicit_value(T value)
    {
        m_ImplicitValue = to_string(value);
        return *this;
    }

    ArgumentParser::Argument &ArgumentParser::Argument::choices(std::initializer_list<std::string> values)
    {
        m_Choices.assign(values.begin(), values.end());
        return *this;
    }

    const std::string &ArgumentParser::Argument::dest() const
    {
        return m_Dest;
    }

    std::string ArgumentParser::Argument::sanitize_dest(const std::string &name)
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
    std::string ArgumentParser::Argument::to_string(const T &value)
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

    bool ArgumentParser::Argument::takes_value() const
    {
        return m_NargsMax > 0;
    }

    bool ArgumentParser::Argument::is_unbounded() const
    {
        return m_NargsMax == std::numeric_limits<std::size_t>::max();
    }

    ArgumentParser::ArgumentParser(std::string program, std::string description, std::string epilog)
        : m_Program(std::move(program)),
          m_Description(std::move(description)),
          m_Epilog(std::move(epilog)),
          m_HelpFormatter(HelpFormatter(80, m_Program))
    {
        // Initialize formatter with default settings
        m_HelpFormatter.program(m_Program)
            .description(m_Description)
            .epilog(m_Epilog);

        if (m_AddHelp)
        {
            add_argument("-h", "--help")
                .action("help")
                .default_value(false)
                .help("show this help message and exit");
        }
    }

    std::string &ArgumentParser::program()
    {
        return m_Program;
    };

    ArgumentParser &ArgumentParser::program(std::string program)
    {
        m_Program = program;
        return *this;
    };

    std::string &ArgumentParser::usage()
    {
        return m_Usage;
    };

    ArgumentParser &ArgumentParser::usage(std::string usage)
    {
        m_Usage = usage;
        return *this;
    };

    std::string &ArgumentParser::epilog()
    {
        return m_Epilog;
    };

    ArgumentParser &ArgumentParser::epilog(std::string epilog)
    {
        m_Epilog = epilog;
        return *this;
    };

    HelpFormatter &ArgumentParser::formatter()
    {
        return m_HelpFormatter;
    };

    ArgumentParser &ArgumentParser::formatter(HelpFormatter formatter)
    {
        m_HelpFormatter = formatter;
        return *this;
    };

    std::string &ArgumentParser::description()
    {
        return m_Description;
    };

    ArgumentParser &ArgumentParser::description(std::string description)
    {
        m_Description = description;
        return *this;
    };

    /**
     * @brief Format help message
     * @return The complete help text
     */
    std::string ArgumentParser::format_help() const
    {
        HelpFormatter fmt = m_HelpFormatter;
        fmt.program(m_Program)
            .usage(m_Usage)
            .description(m_Description)
            .epilog(m_Epilog);

        // Add optional arguments section
        auto &opt_section = const_cast<HelpFormatter &>(fmt).add_section("optional arguments");
        for (const auto &arg : m_Arguments)
        {
            if (arg.m_Kind == Argument::Kind::Optional)
            {
                std::string opt_str;
                for (size_t i = 0; i < arg.m_OptionNames.size(); ++i)
                {
                    if (i > 0)
                        opt_str += ", ";
                    opt_str += arg.m_OptionNames[i];
                }

                std::string metavar = arg.m_Metavar;
                if (metavar.empty() && arg.takes_value())
                {
                    metavar = arg.m_Dest;
                    std::transform(metavar.begin(), metavar.end(), metavar.begin(),
                                   [](unsigned char c)
                                   { return static_cast<char>(std::toupper(c)); });
                }

                opt_section.add_item(opt_str, metavar, arg.m_Help, arg.m_Required);
            }
        }

        // Add positional arguments section if any
        bool has_positional = false;
        for (const auto &arg : m_Arguments)
        {
            if (arg.m_Kind == Argument::Kind::Positional)
            {
                has_positional = true;
                break;
            }
        }

        if (has_positional)
        {
            auto &pos_section = const_cast<HelpFormatter &>(fmt).add_section("positional arguments");
            for (const auto &arg : m_Arguments)
            {
                if (arg.m_Kind == Argument::Kind::Positional)
                {
                    pos_section.add_item(arg.m_Dest, "", arg.m_Help, arg.m_Required);
                }
            }
        }

        // return fmt.format_help(const_cast<std::vector<Argument> *>(&m_Arguments));
        return fmt.format_help();
    }

    /**
     * @brief Format usage message
     * @return The complete usage text
     */
    std::string ArgumentParser::format_usage() const
    {
        HelpFormatter fmt = m_HelpFormatter;
        fmt.program(m_Program)
            .usage(m_Usage)
            .description(m_Description)
            .epilog(m_Epilog);

        // return fmt.format_usage(const_cast<std::vector<Argument> *>(&m_Arguments));
        return fmt.format_usage();
    }

    /**
     * @brief Gets a const reference to the HelpFormatter
     * @return Const reference to the internal HelpFormatter
     */
    const HelpFormatter &ArgumentParser::get_formatter() const
    {
        return m_HelpFormatter;
    }

    template <typename... Names>
    ArgumentParser::Argument &ArgumentParser::add_argument(const Names &...names)
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

    const Namespace &ArgumentParser::parse_args(int argc, char **argv)
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

    /**
     * @brief Prints the help message
     * @param stream to write to (default=stdout).
     */
    void ArgumentParser::print_help(std::ostream &stream) const
    {
        stream << format_help();
    }

    /**
     * @brief Prints the usage message
     * @param stream to write to (default=stdout).
     */
    void ArgumentParser::print_usage(std::ostream &stream) const
    {
        stream << format_usage();
    }

    bool ArgumentParser::is_optional_token(const std::string &token)
    {
        return token.size() > 1 && token[0] == '-';
    }

    std::string ArgumentParser::to_upper(std::string text)
    {
        std::transform(text.begin(), text.end(), text.begin(),
                       [](unsigned char c)
                       { return static_cast<char>(std::toupper(c)); });
        return text;
    }

    std::string ArgumentParser::argument_display(const Argument &arg)
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

    ArgumentParser::Argument &ArgumentParser::lookup_option(const std::string &name)
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

    void ArgumentParser::apply_action(Argument &arg, const std::vector<std::string> &values)
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

    std::vector<std::string> ArgumentParser::collect_option_values(Argument &arg,
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

    void ArgumentParser::consume_long_option(const std::string &token, int &index, int argc, char **argv)
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

    void ArgumentParser::consume_short_cluster(const std::string &token, int &index, int argc, char **argv)
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

    void ArgumentParser::assign_positionals(const std::vector<std::string> &tokens)
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

    void ArgumentParser::finalize_and_validate()
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

    void ArgumentParser::build_namespace()
    {
        m_Namespace.m_Data.clear();
        m_Namespace.m_Provided.clear();

        for (const auto &arg : m_Arguments)
        {
            m_Namespace.m_Data[arg.m_Dest] = arg.m_Values;
            m_Namespace.m_Provided[arg.m_Dest] = arg.m_Occurrences > 0;
        }
    }

} // namespace argparser

#endif // ARGPARSER_IMPLEMENTATION

#pragma endregion // Argparser

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