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
#include <memory>
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

    /**
     * @class ArgumentError
     * @brief Base exception for argument parsing failures.
     * @details Thrown when the parser detects invalid input, invalid values,
     * missing arguments, or unsupported usage patterns.
     */
    class ArgumentError : public std::runtime_error
    {
    public:
        /**
         * @brief Constructs an argument error with a message.
         * @param message Human-readable error description.
         */
        explicit ArgumentError(const std::string &message)
            : std::runtime_error(message) {};
    };

    /**
     * @class ParseError
     * @brief Exception thrown when parsing fails.
     */
    class ParseError : public ArgumentError
    {
    public:
        /**
         * @brief Constructs a parse error with a message.
         * @param message Human-readable error description.
         */
        ParseError(const std::string &message)
            : ArgumentError(message) {};
    };

    /**
     * @class UsageError
     * @brief Exception thrown when the parser is used incorrectly.
     */
    class UsageError : public ArgumentError
    {
    public:
        /**
         * @brief Constructs a usage error with a message.
         * @param message Human-readable error description.
         */
        UsageError(const std::string &message)
            : ArgumentError(message) {};
    };

    /**
     * @class RequiredError
     * @brief Exception thrown when a required argument or value is missing.
     */
    class RequiredError : public ArgumentError
    {
    public:
        /**
         * @brief Constructs a required-argument error with a message.
         * @param message Human-readable error description.
         */
        RequiredError(const std::string &message)
            : ArgumentError(message) {};
    };

    /**
     * @class HelpError
     * @brief Exception thrown when help output is requested.
     */
    class HelpError : public ArgumentError
    {
    public:
        /**
         * @brief Constructs the help-request signal exception.
         */
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
         * @enum Color
         * @brief ANSI color and text-style codes used in colored output.
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
         * @enum Format
         * @brief Supported help output formats.
         */
        enum class Format
        {
            Plain,    ///< Plain text without colors
            Colored,  ///< Text with ANSI colors
            Markdown, ///< Markdown format
            Compact   ///< Compact format (single-line descriptions)
        };

        /**
         * @struct ColorTheme
         * @brief Color scheme for help messages.
         */
        struct ColorTheme
        {
            bool enabled = true;                      ///< Enables ANSI color output.
            Color usage_color = Color::Green;         ///< Color for usage headers.
            Color option_color = Color::Blue;         ///< Color for option names.
            Color warnings_color = Color::Red;        ///< Color for warning sections.
            Color required_color = Color::Red;        ///< Color for required markers.
            Color heading_color = Color::Bold;        ///< Color for section headings.
            Color section_color = Color::Cyan;        ///< Color for section labels.
            Color notes_color = Color::Magenta;       ///< Color for notes text.
            Color examples_color = Color::Green;      ///< Color for examples.
            Color metavar_color = Color::Yellow;      ///< Color for metavariables.
            Color description_color = Color::Default; ///< Color for descriptions.
        };

        /**
         * @struct Alignment
         * @brief Layout and wrapping configuration for help text.
         */
        struct Alignment
        {
            size_t indent_size = 2;            ///< Number of characters per indent level.
            char indent_char = ' ';            ///< Character used for indentation.
            size_t option_width = 20;          ///< Desired width of the option column.
            bool wrap_help_text = true;        ///< Enables wrapping of descriptions.
            size_t max_help_position = 24;     ///< Maximum column for help text start.
            size_t min_description_width = 20; ///< Minimum width reserved for descriptions.
        };

        /**
         * @class Section
         * @brief Logical grouping of help content in a generated help message.
         */
        class Section
        {
        public:
            /**
             * @enum Type
             * @brief Section classification used by help formatters.
             */
            enum class Type
            {
                Notes,     ///< Additional notes
                Custom,    ///< Custom section
                Examples,  ///< Usage examples
                Warnings,  ///< Warning messages
                Arguments, ///< Arguments section
            };

            /**
             * @brief Constructs a help section.
             * @param title Section title.
             * @param description Optional section description.
             * @param type Section category.
             */
            Section(const std::string &title = "", const std::string &description = "",
                    Type type = Type::Custom);

            /**
             * @brief Adds a formatted item to the section.
             * @param option Option text to display.
             * @param metavar Value placeholder text.
             * @param help Human-readable help text.
             * @param required Whether the item should be marked required.
             * @return Reference to this section.
             */
            Section &add_item(const std::string &option, const std::string &metavar,
                              const std::string &help, bool required = false);

            /**
             * @brief Adds a free-form text block to the section.
             * @param text Text to append.
             * @return Reference to this section.
             */
            Section &add_text(const std::string &text);
            /**
             * @brief Adds a usage example to the section.
             * @param command Example command line.
             * @param description Optional example description.
             * @return Reference to this section.
             */
            Section &add_example(const std::string &command, const std::string &description);
            /**
             * @brief Indicates whether the section is visible.
             * @return True if visible, false otherwise.
             */
            bool visible();
            /**
             * @brief Sets the section visibility.
             * @param visible Whether the section should be visible.
             * @return Reference to this section.
             */
            Section &visible(bool visible);
            /**
             * @brief Returns the section title.
             * @return The title string.
             */
            const std::string &title() const;
            /**
             * @brief Returns the section description.
             * @return The description string.
             */
            const std::string &description() const;
            /**
             * @brief Returns the section type.
             * @return The section category.
             */
            Type type() const;
            /**
             * @brief Indicates whether the section is visible.
             * @return True if visible, false otherwise.
             */
            bool visible() const;

        private:
            friend class HelpFormatter;

            /**
             * @struct Item
             * @brief A single option/help row in a section.
             */
            struct Item
            {
                std::string option;  ///< Displayed option text.
                std::string metavar; ///< Placeholder or value label.
                std::string help;    ///< Human-readable description.
                bool required;       ///< Marks the item as required.
            };

            /**
             * @struct Example
             * @brief A usage example row in a section.
             */
            struct Example
            {
                std::string command;     ///< Example command line.
                std::string description; ///< Optional explanation.
            };

            std::string m_Title;              ///< Section title.
            std::string m_Description;        ///< Section description.
            Type m_Type;                      ///< Section category.
            bool m_Visible = true;            ///< Visibility flag.
            std::vector<Item> m_Items;        ///< Item rows.
            std::vector<std::string> m_Texts; ///< Free-form text blocks.
            std::vector<Example> m_Examples;  ///< Example rows.
        };

    public:
        /**
         * @brief Constructs a help formatter.
         * @param width Output width in characters. Use 0 for auto-detection.
         * @param prog_name Program name used in generated usage text.
         */
        HelpFormatter(size_t width = 80, const std::string &prog_name = "");
        /**
         * @brief Sets the program name.
         * @param name Program name.
         * @return Reference to this formatter.
         */
        HelpFormatter &program(const std::string &name);
        /**
         * @brief Sets the usage string.
         * @param usage Usage text.
         * @return Reference to this formatter.
         */
        HelpFormatter &usage(const std::string &usage);
        /**
         * @brief Sets the epilog text.
         * @param epilog Footer text shown after help sections.
         * @return Reference to this formatter.
         */
        HelpFormatter &epilog(const std::string &epilog);
        /**
         * @brief Sets the description text.
         * @param description Program description.
         * @return Reference to this formatter.
         */
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

        /**
         * @brief Enables or disables section sorting.
         * @param sort True to sort sections.
         * @return Reference to this formatter.
         */
        HelpFormatter &sort_sections(bool sort);

        // === Section Management ===

        /**
         * @brief Adds a new section to the formatter.
         * @param title Section title.
         * @param description Section description.
         * @param type Section category.
         * @return Reference to the created section.
         */
        Section &add_section(const std::string &title, const std::string &description = "",
                             Section::Type type = Section::Type::Custom);

        /**
         * @brief Returns an existing section or creates it if needed.
         * @param title Section title.
         * @param description Section description.
         * @param type Section category.
         * @return Reference to the matching section.
         */
        Section &get_section(const std::string &title, const std::string &description = "",
                             Section::Type type = Section::Type::Custom);

        // === Getter Methods ===

        /** @brief Returns the configured program name. */
        const std::string &program() const;
        /** @brief Returns the configured output width. */
        size_t width() const;
        /** @brief Returns mutable access to the current color theme. */
        ColorTheme &color_theme();
        /** @brief Returns read-only access to the current color theme. */
        const ColorTheme &color_theme() const;
        /** @brief Returns mutable access to the alignment configuration. */
        Alignment &alignment();
        /** @brief Returns read-only access to the alignment configuration. */
        const Alignment &alignment() const;
        /** @brief Returns the configured usage string. */
        const std::string &usage() const;
        /** @brief Returns the configured epilog string. */
        const std::string &epilog() const;
        /** @brief Returns the configured description string. */
        const std::string &description() const;

        // === Formatting Methods ===

        /** @brief Builds the complete help message. */
        std::string format_help() const;

        /** @brief Builds the usage line. */
        std::string format_usage() const;
        /** @brief Formats a single section. */
        std::string format_section(const Section &section) const;
        /** @brief Wraps a block of text to the configured width. */
        std::string wrap_text(const std::string &text, size_t indent_level = 0) const;
        /** @brief Applies ANSI color formatting to text when enabled. */
        std::string colorize(const std::string &text, Color color) const;

    private:
        /** @brief Detects the terminal width when auto-sizing is requested. */
        static size_t get_terminal_width();

        /** @brief Maps a theme color to the active output format. */
        Color get_color_for_format(Color default_color) const;
        /** @brief Wraps and colors text as needed. */
        std::string format_colored(const std::string &text, Color color) const;
        /** @brief Wraps a sequence of words into lines. */
        std::string wrap_words(const std::vector<std::string> &words, size_t indent_level) const;
        /** @brief Formats a single argument row. */
        std::string format_item(const Section::Item &item) const;
        /** @brief Formats a single example row. */
        std::string format_example(const Section::Example &example) const;

    private:
        size_t m_Width;                  ///< Output width in characters.
        std::string m_Program;           ///< Program name used for usage text.
        std::string m_Usage;             ///< Explicit usage string.
        std::string m_Epilog;            ///< Footer text.
        std::string m_Description;       ///< Main description text.
        std::string m_PrefixInfo;        ///< Pre-usage text block.
        std::string m_SuffixInfo;        ///< Post-help text block.
        std::string m_LineSeparator;     ///< Separator line used between sections.
        bool m_SortSections = false;     ///< Whether sections are sorted.
        std::vector<Section> m_Sections; ///< Help sections.
        ColorTheme m_Theme;              ///< Current color theme.
        Alignment m_Alignment;           ///< Current layout settings.
        Format m_Format;                 ///< Current output format.
    };

    /**
     * @class ArgumentParser
     * @brief Python-style command line parser for C++17.
     *
     * Features include optional/positional arguments, actions, default values,
     * choices validation, environment fallback, and subcommands.
     */
    class ArgumentParser
    {
    public:
        /**
         * @class Namespace
         * @brief Parsed argument values container returned by ArgumentParser.
         * @details Provides typed lookup, raw access, presence tracking, and
         * optional subcommand results.
         */
        class Namespace
        {
            friend class ArgumentParser;

        public:
            /** @brief Constructs an empty namespace. */
            Namespace() = default;
            /** @brief Copies a namespace. */
            Namespace(const Namespace &other);
            /** @brief Copy-assigns a namespace. */
            Namespace &operator=(const Namespace &other);
            /** @brief Moves a namespace. */
            Namespace(Namespace &&) noexcept = default;
            /** @brief Move-assigns a namespace. */
            Namespace &operator=(Namespace &&) noexcept = default;
            /** @brief Destroys the namespace. */
            ~Namespace() = default;

            /** @brief Returns whether an argument exists in the namespace. */
            bool has(const std::string &name) const;
            /** @brief Returns whether an argument was explicitly provided. */
            bool provided(const std::string &name) const;
            /** @brief Returns the raw parsed values for an argument. */
            const std::vector<std::string> &raw_values(const std::string &name) const;
            /** @brief Returns whether a subcommand was parsed. */
            bool has_subcommand() const;
            /** @brief Returns the parsed subcommand name. */
            const std::string &subcommand() const;
            /** @brief Returns the parsed namespace for the active subcommand. */
            const Namespace &subcommand_namespace() const;
            /** @brief Returns a typed value for an argument. */
            template <typename T>
            T get(const std::string &name) const;
            /** @brief Returns a typed list of all values for an argument. */
            template <typename T>
            std::vector<T> getlist(const std::string &name) const;

        private:
            /** @brief Converts a raw string to the requested type. */
            template <typename T>
            static T convert(const std::string &value);

        private:
            std::unordered_map<std::string, std::vector<std::string>> m_Data; ///< Stored argument values.
            std::unordered_map<std::string, bool> m_Provided;                 ///< Explicitly provided flags.
            std::string m_Subcommand;                                         ///< Parsed subcommand name.
            std::unique_ptr<Namespace> m_SubcommandNamespace;                 ///< Parsed subcommand namespace.
        };

        /**
         * @class Argument
         * @brief Describes a single positional or optional argument.
         */
        class Argument
        {
            friend class ArgumentParser;

            /** @brief Internal argument kind. */
            enum class Kind
            {
                Positional,
                Optional
            };

            /** @brief Internal action type applied after parsing. */
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
            /** @brief Sets help text for the argument. */
            Argument &help(const std::string &text);
            /**
             * @brief Mark argument as hidden from help output.
             * @param value True to hide, false to show.
             */
            /** @brief Hides or reveals the argument in help output. */
            Argument &hidden(bool value = true);
            /** @brief Marks the argument as required. */
            Argument &required(bool value = true);
            /** @brief Sets the metavar text shown in help output. */
            Argument &metavar(const std::string &text);
            /** @brief Sets the action performed when the argument is parsed. */
            Argument &action(const std::string &name);
            /** @brief Sets the exact number of values accepted. */
            Argument &nargs(std::size_t count);
            /** @brief Sets a Python-style nargs pattern such as ?, *, or +. */
            Argument &nargs(char pattern);
            /** @brief Sets a default value that is used when no CLI value is provided. */
            template <typename T>
            Argument &default_value(T value);
            /** @brief Sets multiple default values. */
            Argument &default_values(std::initializer_list<std::string> values);
            /** @brief Sets an implicit value used when the argument is present without data. */
            template <typename T>
            Argument &implicit_value(T value);
            /**
             * @brief Read value from an environment variable when not provided on CLI.
             * @param name Environment variable name.
             */
            /** @brief Reads a fallback value from an environment variable. */
            Argument &env(const std::string &name);
            /** @brief Restricts the argument to a set of allowed string values. */
            Argument &choices(std::initializer_list<std::string> values);
            /** @brief Returns the canonical destination name. */
            const std::string &dest() const;

        private:
            /** @brief Converts an option name into a canonical destination token. */
            static std::string sanitize_dest(const std::string &name);
            /** @brief Converts a typed value to a string. */
            template <typename T>
            static std::string to_string(const T &value);
            /** @brief Returns whether this argument stores values. */
            bool takes_value() const;
            /** @brief Returns whether this argument accepts unlimited values. */
            bool is_unbounded() const;

        private:
            Kind m_Kind{Kind::Positional};          ///< Positional or optional argument.
            ActionType m_Action{ActionType::Store}; ///< Selected action behavior.

            std::vector<std::string> m_OptionNames; ///< Option aliases such as -v and --verbose.
            std::string m_Dest;                     ///< Canonical destination name.
            std::string m_Help;                     ///< Help string.
            std::string m_Metavar;                  ///< Placeholder shown in help.

            std::size_t m_NargsMin{1}; ///< Minimum number of values accepted.
            std::size_t m_NargsMax{1}; ///< Maximum number of values accepted.

            bool m_Required{false}; ///< Whether the argument is required.

            std::vector<std::string> m_DefaultValues;   ///< Default fallback values.
            std::optional<std::string> m_ImplicitValue; ///< Implicit value when no explicit value is given.
            std::optional<std::string> m_EnvVar;        ///< Environment variable fallback name.
            std::vector<std::string> m_Choices;         ///< Allowed values list.
            bool m_Hidden{false};                       ///< Hidden from help output.

            std::vector<std::string> m_Values; ///< Parsed values.
            std::size_t m_Occurrences{0};      ///< Number of times the argument appeared.
        };

    public:
        /**
         * @brief Constructs an argument parser.
         * @param program Program name.
         * @param description Parser description.
         * @param epilog Footer text shown after help output.
         */
        ArgumentParser(std::string program = "", std::string description = "", std::string epilog = "");
        /** @brief Returns a mutable reference to the program name. */
        std::string &program();
        /** @brief Sets the program name. */
        ArgumentParser &program(std::string program);
        /** @brief Returns a mutable reference to the usage string. */
        std::string &usage();
        /** @brief Sets the usage string. */
        ArgumentParser &usage(std::string usage);
        /** @brief Returns a mutable reference to the epilog string. */
        std::string &epilog();
        /** @brief Sets the epilog string. */
        ArgumentParser &epilog(std::string epilog);
        /** @brief Returns the internal help formatter. */
        HelpFormatter &formatter();
        /** @brief Replaces the internal help formatter. */
        ArgumentParser &formatter(HelpFormatter formatter);
        /** @brief Returns a mutable reference to the description string. */
        std::string &description();
        /** @brief Returns the current description string. */
        const std::string &description() const;
        /** @brief Sets the description string. */
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
        /** @brief Returns a read-only reference to the current help formatter. */
        const HelpFormatter &get_formatter() const;

        /**
         * @brief Adds a positional or optional argument.
         * @param names One or more argument names or aliases.
         * @return Reference to the newly created argument.
         */
        template <typename... Names>
        Argument &add_argument(const Names &...names);

        /**
         * @brief Add or retrieve a subcommand parser by name.
         * @param name Subcommand token (e.g. "init", "build").
         * @param description Description shown in the parent help.
         * @return Reference to the subcommand parser.
         */
        ArgumentParser &add_subcommand(const std::string &name, const std::string &description = "");

        /**
         * @brief Parse arguments and throw on unknown options.
         */
        /** @brief Parses arguments and throws on unknown tokens. */
        const Namespace &parse_args(int argc, char **argv);

        /**
         * @brief Parse arguments and preserve unknown options/tokens.
         */
        /** @brief Parses arguments and stores unknown tokens for later inspection. */
        const Namespace &parse_known_args(int argc, char **argv);

        /**
         * @brief Unknown options/tokens captured by parse_known_args().
         */
        /** @brief Returns the unknown arguments captured by parse_known_args(). */
        const std::vector<std::string> &unknown_args() const;

        /**
         * @brief Prints the help message
         * @param stream to write to (default=stdout).
         */
        /** @brief Writes the generated help text to a stream. */
        void print_help(std::ostream &stream = std::cout) const;

        /**
         * @brief Prints the usage message
         * @param stream to write to (default=stdout).
         */
        /** @brief Writes the generated usage text to a stream. */
        void print_usage(std::ostream &stream = std::cout) const;

    private:
        /** @brief Returns true for numeric tokens that should be treated as values. */
        static bool is_negative_number_token(const std::string &token);
        /** @brief Determines whether a token should be parsed as an option. */
        bool should_treat_as_optional(const std::string &token) const;
        /** @brief Converts text to upper-case. */
        static std::string to_upper(std::string text);
        /** @brief Builds a display string for an argument. */
        static std::string argument_display(const Argument &arg);
        /** @brief Finds a registered option or throws. */
        Argument &lookup_option(const std::string &name);
        /** @brief Finds a registered option or returns nullptr. */
        Argument *try_lookup_option(const std::string &name);
        /** @brief Applies the configured action for a parsed argument. */
        void apply_action(Argument &arg, const std::vector<std::string> &values);
        std::vector<std::string> collect_option_values(Argument &arg,
                                                       int &index,
                                                       int argc,
                                                       char **argv,
                                                       std::optional<std::string> first);
        /** @brief Shared parse routine used by strict and permissive modes. */
        void parse_impl(int argc, char **argv, bool allow_unknown);

        /** @brief Parses a long option token. */
        void consume_long_option(const std::string &token, int &index, int argc, char **argv, bool allow_unknown);
        /** @brief Parses a short option cluster token. */
        void consume_short_cluster(const std::string &token, int &index, int argc, char **argv, bool allow_unknown);
        /** @brief Assigns positional tokens to positional arguments. */
        void assign_positionals(const std::vector<std::string> &tokens);
        /** @brief Applies defaults, environment values, and validation rules. */
        void finalize_and_validate();
        /** @brief Writes the parsed state into the namespace object. */
        void build_namespace();

    private:
        std::string m_Usage;       ///< User-configured usage string.
        std::string m_Epilog;      ///< Footer text.
        std::string m_Program;     ///< Program name.
        std::string m_Description; ///< Parser description.

        bool m_AddHelp{true}; ///< Whether the built-in help option is added.

        std::vector<Argument> m_Arguments;                                              ///< All registered arguments.
        std::vector<std::size_t> m_Positionals;                                         ///< Indices of positional arguments.
        std::unordered_map<std::string, std::size_t> m_OptionLookup;                    ///< Lookup table for option aliases.
        std::unordered_map<std::string, std::unique_ptr<ArgumentParser>> m_Subcommands; ///< Registered subcommands.
        std::vector<std::string> m_UnknownArgs;                                         ///< Unknown tokens captured during parsing.

        Namespace m_Namespace;         ///< Last parsed namespace.
        HelpFormatter m_HelpFormatter; ///< Formatter used for help and usage.
    };

    /** @brief Alias for HelpFormatter. */
    using Formatter = HelpFormatter;
    /** @brief Alias for ArgumentParser. */
    using Argparser = ArgumentParser;
    /** @brief Alias for HelpFormatter::Color. */
    using Color = HelpFormatter::Color;
    /** @brief Alias for HelpFormatter::Section. */
    using Section = HelpFormatter::Section;
    /** @brief Alias for HelpFormatter::Alignment. */
    using Alignment = HelpFormatter::Alignment;
    /** @brief Alias for HelpFormatter::ColorTheme. */
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
    // [Class] Section
    //-----------------------------------------------------------------------------
    // - Section()
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

    //-----------------------------------------------------------------------------
    // [Class] ArgumentParser
    //-----------------------------------------------------------------------------
    // - ArgumentParser()
    //-----------------------------------------------------------------------------

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

    const std::string &ArgumentParser::description() const
    {
        return m_Description;
    }

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
            if (arg.m_Kind == Argument::Kind::Optional && !arg.m_Hidden)
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
            if (arg.m_Kind == Argument::Kind::Positional && !arg.m_Hidden)
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
                if (arg.m_Kind == Argument::Kind::Positional && !arg.m_Hidden)
                {
                    pos_section.add_item(arg.m_Dest, "", arg.m_Help, arg.m_Required);
                }
            }
        }

        if (!m_Subcommands.empty())
        {
            auto &sub_section = const_cast<HelpFormatter &>(fmt).add_section("subcommands");
            for (const auto &entry : m_Subcommands)
            {
                const auto &name = entry.first;
                const auto &parser = *entry.second;
                sub_section.add_item(name, "", parser.description(), false);
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

    ArgumentParser &ArgumentParser::add_subcommand(const std::string &name, const std::string &description)
    {
        if (name.empty() || name[0] == '-')
        {
#ifndef ARGPARSER_NO_EXCEPTIONS
            throw ArgumentError("subcommand must be a non-empty token not starting with '-': " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
        }

        auto it = m_Subcommands.find(name);
        if (it == m_Subcommands.end())
        {
            auto parser = std::make_unique<ArgumentParser>(name, description, "");
            it = m_Subcommands.emplace(name, std::move(parser)).first;
        }
        else
        {
            it->second->description(description);
        }

        return *it->second;
    }

    const ArgumentParser::Namespace &ArgumentParser::parse_args(int argc, char **argv)
    {
        parse_impl(argc, argv, false);
        return m_Namespace;
    }

    const ArgumentParser::Namespace &ArgumentParser::parse_known_args(int argc, char **argv)
    {
        parse_impl(argc, argv, true);
        return m_Namespace;
    }

    const std::vector<std::string> &ArgumentParser::unknown_args() const
    {
        return m_UnknownArgs;
    }

    void ArgumentParser::parse_impl(int argc, char **argv, bool allow_unknown)
    {
        if (m_Program.empty() && argc > 0 && argv != nullptr)
        {
            m_Program = argv[0];
        }

        m_UnknownArgs.clear();
        m_Namespace.m_Subcommand.clear();
        m_Namespace.m_SubcommandNamespace.reset();

        for (auto &arg : m_Arguments)
        {
            arg.m_Values.clear();
            arg.m_Occurrences = 0;
        }

        std::vector<std::string> positional_tokens;

        int subcommand_index = -1;
        std::string subcommand_name;
        if (!m_Subcommands.empty())
        {
            for (int s = 1; s < argc; ++s)
            {
                const std::string token = argv[s];
                if (token == "--")
                {
                    break;
                }
                if (!should_treat_as_optional(token))
                {
                    const auto it = m_Subcommands.find(token);
                    if (it != m_Subcommands.end())
                    {
                        subcommand_index = s;
                        subcommand_name = token;
                        break;
                    }
                }
            }
        }

        const int scan_end = (subcommand_index >= 0) ? subcommand_index : argc;

        int i = 1;
        while (i < scan_end)
        {
            const std::string token = argv[i];
            if (token == "--")
            {
                ++i;
                while (i < scan_end)
                {
                    positional_tokens.emplace_back(argv[i]);
                    ++i;
                }
                break;
            }

            if (should_treat_as_optional(token))
            {
                if (token.rfind("--", 0) == 0)
                {
                    consume_long_option(token, i, scan_end, argv, allow_unknown);
                }
                else
                {
                    consume_short_cluster(token, i, scan_end, argv, allow_unknown);
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

        if (subcommand_index >= 0)
        {
            auto sub_it = m_Subcommands.find(subcommand_name);
            if (sub_it != m_Subcommands.end())
            {
                auto &subparser = *sub_it->second;
                std::vector<std::string> sub_tokens;
                sub_tokens.reserve(static_cast<std::size_t>(argc - subcommand_index));
                sub_tokens.push_back(m_Program.empty() ? subcommand_name : (m_Program + " " + subcommand_name));
                for (int j = subcommand_index + 1; j < argc; ++j)
                {
                    sub_tokens.emplace_back(argv[j]);
                }

                std::vector<char *> sub_argv;
                sub_argv.reserve(sub_tokens.size());
                for (auto &token : sub_tokens)
                {
                    sub_argv.push_back(const_cast<char *>(token.c_str()));
                }

                const Namespace &sub_ns = allow_unknown
                                              ? subparser.parse_known_args(static_cast<int>(sub_argv.size()), sub_argv.data())
                                              : subparser.parse_args(static_cast<int>(sub_argv.size()), sub_argv.data());

                m_Namespace.m_Subcommand = subcommand_name;
                m_Namespace.m_SubcommandNamespace = std::make_unique<Namespace>(sub_ns);

                if (allow_unknown)
                {
                    for (const auto &unknown : subparser.unknown_args())
                    {
                        m_UnknownArgs.push_back(unknown);
                    }
                }
            }
        }
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

    bool ArgumentParser::is_negative_number_token(const std::string &token)
    {
        if (token.size() < 2 || token[0] != '-')
        {
            return false;
        }

        std::size_t idx = 1;
        bool has_digit = false;

        while (idx < token.size() && std::isdigit(static_cast<unsigned char>(token[idx])))
        {
            has_digit = true;
            ++idx;
        }

        if (idx < token.size() && token[idx] == '.')
        {
            ++idx;
            while (idx < token.size() && std::isdigit(static_cast<unsigned char>(token[idx])))
            {
                has_digit = true;
                ++idx;
            }
        }

        if (!has_digit)
        {
            return false;
        }

        if (idx < token.size() && (token[idx] == 'e' || token[idx] == 'E'))
        {
            ++idx;
            if (idx < token.size() && (token[idx] == '+' || token[idx] == '-'))
            {
                ++idx;
            }

            bool has_exp_digit = false;
            while (idx < token.size() && std::isdigit(static_cast<unsigned char>(token[idx])))
            {
                has_exp_digit = true;
                ++idx;
            }

            if (!has_exp_digit)
            {
                return false;
            }
        }

        return idx == token.size();
    }

    bool ArgumentParser::should_treat_as_optional(const std::string &token) const
    {
        if (token.size() < 2 || token[0] != '-')
        {
            return false;
        }
        if (token == "-")
        {
            return false;
        }
        if (is_negative_number_token(token))
        {
            return false;
        }
        return true;
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
        auto *arg = try_lookup_option(name);
        if (arg == nullptr)
        {
#ifndef ARGPARSER_NO_EXCEPTIONS
            throw ParseError("unrecognized argument: " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
        }
        return *arg;
    }

    ArgumentParser::Argument *ArgumentParser::try_lookup_option(const std::string &name)
    {
        const auto it = m_OptionLookup.find(name);
        if (it == m_OptionLookup.end())
        {
            return nullptr;
        }
        return &m_Arguments[it->second];
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
            if (should_treat_as_optional(next))
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
            if (should_treat_as_optional(next))
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

    void ArgumentParser::consume_long_option(const std::string &token, int &index, int argc, char **argv, bool allow_unknown)
    {
        const auto eq = token.find('=');
        const std::string name = (eq == std::string::npos) ? token : token.substr(0, eq);
        const std::optional<std::string> inline_value = (eq == std::string::npos)
                                                            ? std::nullopt
                                                            : std::optional<std::string>(token.substr(eq + 1));

        auto *arg = try_lookup_option(name);
        if (arg == nullptr)
        {
            if (allow_unknown)
            {
                m_UnknownArgs.push_back(token);
                return;
            }
#ifndef ARGPARSER_NO_EXCEPTIONS
            throw ParseError("unrecognized argument: " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
        }
        const auto values = collect_option_values(*arg, index, argc, argv, inline_value);
        apply_action(*arg, values);
    }

    void ArgumentParser::consume_short_cluster(const std::string &token, int &index, int argc, char **argv, bool allow_unknown)
    {
        for (std::size_t pos = 1; pos < token.size(); ++pos)
        {
            const std::string name = std::string("-") + token[pos];
            auto *arg = try_lookup_option(name);
            if (arg == nullptr)
            {
                if (allow_unknown)
                {
                    m_UnknownArgs.push_back(token.substr(pos - 1));
                    return;
                }
#ifndef ARGPARSER_NO_EXCEPTIONS
                throw ParseError("unrecognized argument: " + name);
#endif // ARGPARSER_NO_EXCEPTIONS
            }

            std::optional<std::string> inline_value;
            if (arg->takes_value() && pos + 1 < token.size())
            {
                inline_value = token.substr(pos + 1);
                pos = token.size();
            }

            const auto values = collect_option_values(*arg, index, argc, argv, inline_value);
            apply_action(*arg, values);

            if (arg->takes_value())
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
                else if (arg.m_EnvVar.has_value())
                {
                    const char *env_value = std::getenv(arg.m_EnvVar->c_str());
                    if (env_value != nullptr)
                    {
                        arg.m_Values = {std::string(env_value)};
                    }
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

    //-----------------------------------------------------------------------------
    // [Class] Namespace
    //-----------------------------------------------------------------------------
    // - Namespace()
    //-----------------------------------------------------------------------------

    ArgumentParser::Namespace::Namespace(const Namespace &other)
        : m_Data(other.m_Data),
          m_Provided(other.m_Provided),
          m_Subcommand(other.m_Subcommand)
    {
        if (other.m_SubcommandNamespace)
        {
            m_SubcommandNamespace = std::make_unique<Namespace>(*other.m_SubcommandNamespace);
        }
    }

    ArgumentParser::Namespace &ArgumentParser::Namespace::operator=(const Namespace &other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_Data = other.m_Data;
        m_Provided = other.m_Provided;
        m_Subcommand = other.m_Subcommand;

        if (other.m_SubcommandNamespace)
        {
            m_SubcommandNamespace = std::make_unique<ArgumentParser::Namespace>(*other.m_SubcommandNamespace);
        }
        else
        {
            m_SubcommandNamespace.reset();
        }

        return *this;
    }

    bool ArgumentParser::Namespace::has(const std::string &name) const
    {
        return m_Data.find(name) != m_Data.end();
    }

    bool ArgumentParser::Namespace::provided(const std::string &name) const
    {
        const auto it = m_Provided.find(name);
        return it != m_Provided.end() && it->second;
    }

    const std::vector<std::string> &ArgumentParser::Namespace::raw_values(const std::string &name) const
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

    bool ArgumentParser::Namespace::has_subcommand() const
    {
        return !m_Subcommand.empty();
    }

    const std::string &ArgumentParser::Namespace::subcommand() const
    {
        return m_Subcommand;
    }

    const ArgumentParser::Namespace &ArgumentParser::Namespace::subcommand_namespace() const
    {
        if (!m_SubcommandNamespace)
        {
#ifndef ARGPARSER_NO_EXCEPTIONS
            throw ParseError("subcommand namespace not available");
#endif // ARGPARSER_NO_EXCEPTIONS
        }
        return *m_SubcommandNamespace;
    }

    template <typename T>
    T ArgumentParser::Namespace::get(const std::string &name) const
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
    std::vector<T> ArgumentParser::Namespace::getlist(const std::string &name) const
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
    T ArgumentParser::Namespace::convert(const std::string &value)
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
    // [Class] Argument
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

    ArgumentParser::Argument &ArgumentParser::Argument::help(const std::string &text)
    {
        m_Help = text;
        return *this;
    };

    ArgumentParser::Argument &ArgumentParser::Argument::hidden(bool value)
    {
        m_Hidden = value;
        return *this;
    }

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

    ArgumentParser::Argument &ArgumentParser::Argument::env(const std::string &name)
    {
        m_EnvVar = name;
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