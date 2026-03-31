<p align="center"><h1 align="center">ARGPARSER</h1></p>
<p align="center">
  <em><code>Modern C/C++ Argument Parser.
</code></em>
</p>
<p align="center">
  <img src="https://img.shields.io/github/license/djoezeke/argparser?style=default&logo=opensourceinitiative&logoColor=white&color=0080ff" alt="license">
  <img src="https://img.shields.io/github/last-commit/djoezeke/argparser?style=default&logo=git&logoColor=white&color=0080ff" alt="last-commit">
  <img src="https://img.shields.io/github/languages/top/djoezeke/argparser?style=default&color=0080ff" alt="repo-top-language">
  <img src="https://img.shields.io/github/languages/count/djoezeke/argparser?style=default&color=0080ff" alt="repo-language-count">
</p>
<p align="center"><!-- default option, no dependency badges. -->
</p>
<br>

<details><summary>Table of Contents</summary>

- [📍 Overview](#-overview)
- [✨ Features](#-features)
- [🚀 Getting Started](#-getting-started)
  - [☑️ Prerequisites](#-prerequisites)
  - [⚙️ Installation](#-installation)
  - [🤖 Usage](#🤖-usage)
  - [🧪 Testing](#🧪-testing)
- [🔰 Contributing](#-contributing)
- [🙌 Acknowledgments](#-acknowledgments)
- [📃 License](#-license)

</details>

## 📍 Overview

`Argparser` is a single-file, header-only C++ argument parsing library inspired by Python's `argparse`.
It offers a modern and intuitive API for declaring arguments with `add_argument(...)`, parsing CLI input, and printing help text.

## ✨ Features

- Header-only C++17 (single include)
- Python-style `add_argument` API
- Positional + optional arguments
- Actions: `store_true`, `store_false`, `append`, `count`
- Default values, required args, and formatted help messages
- Subcommands via `add_subcommand(...)`
- Unknown argument capture via `parse_known_args(...)`
- Environment fallback values via `.env("NAME")`
- Better negative-number handling (`-42`, `-3.14`) as values

Argparser distinguishes 3 different types of arguments:

| Type    | Functin   | Description                                                             |
| ------- | --------- | ----------------------------------------------------------------------- |
| `arg`   | add_arg   | named positional arguments (e.g. file)                                  |
| `flag`  | add_flag  | a boolean argument that is by default false (e.g. --verbose)            |
| `kwarg` | add_kwarg | keyworded-arguments that require a key and a value, (e.g. --variable=5) |

### 📝 Supported Syntax

```
--help --verbose --input=file.txt
-hvi=file.txt
-h --verbose -i file.txt
--long value
```

## 🚀 Getting Started

### ☑️ Prerequisites

| Compiler | Min Version |
| :------: | :---------: |
| GNU G++  |     13      |
|  Clang   |     17      |

### ⚙️ Installation

To use Argparser in your project, include the single header:

```cpp
#include "argparser.hpp"
```

With CMake (as subdirectory):

```cmake
add_subdirectory(path/to/argparser)
target_link_libraries(your_app PRIVATE Argparser::Argparser)
```

With CMake (installed package):

```cmake
find_package(Argparser CONFIG REQUIRED)
target_link_libraries(your_app PRIVATE Argparser::Argparser)
```

Install locally:

```sh
cmake -S . -B build -DARGPARSER_ENABLE_INSTALL=ON
cmake --build build
cmake --install build --prefix ./install
```

Configure project options:

- `ARGPARSER_BUILD_EXAMPLES=ON|OFF`
- `ARGPARSER_BUILD_TESTS=ON|OFF`
- `ARGPARSER_ENABLE_INSTALL=ON|OFF`
- `ARGPARSER_ENABLE_DOXYGEN=ON|OFF`

### 🤖 Usage

<!--
*    [Quick Start](#quick-start)
     *    [Positional Arguments](#positional-arguments)
     *    [Optional Arguments](#optional-arguments)
          *    [Requiring optional arguments](#requiring-optional-arguments)
          *    [Accessing optional arguments without default values](#accessing-optional-arguments-without-default-values)
          *    [Deciding if the value was given by the user](#deciding-if-the-value-was-given-by-the-user)
          *    [Joining values of repeated optional arguments](#joining-values-of-repeated-optional-arguments)
          *    [Repeating an argument to increase a value](#repeating-an-argument-to-increase-a-value)
          *    [Mutually Exclusive Group](#mutually-exclusive-group)
     *    [Storing values into variables](#store-into)
     *    [Negative Numbers](#negative-numbers)
     *    [Combining Positional and Optional Arguments](#combining-positional-and-optional-arguments)
     *    [Printing Help](#printing-help)
     *    [Adding a description and an epilog to help](#adding-a-description-and-an-epilog-to-help)
     *    [List of Arguments](#list-of-arguments)
     *    [Compound Arguments](#compound-arguments)
     *    [Converting to Numeric Types](#converting-to-numeric-types)
     *    [Default Arguments](#default-arguments)
     *    [Gathering Remaining Arguments](#gathering-remaining-arguments)
     *    [Parent Parsers](#parent-parsers)
     *    [Subcommands](#subcommands)
     *    [Getting Argument and Subparser Instances](#getting-argument-and-subparser-instances)
     *    [Parse Known Args](#parse-known-args)
     *    [Hidden argument and alias](#hidden-argument-alias)
     *    [ArgumentParser in bool Context](#argumentparser-in-bool-context)
     *    [Custom Prefix Characters](#custom-prefix-characters)
     *    [Custom Assignment Characters](#custom-assignment-characters)
*    [Further Examples](#further-examples)
     *    [Construct a JSON object from a filename argument](#construct-a-json-object-from-a-filename-argument)
     *    [Positional Arguments with Compound Toggle Arguments](#positional-arguments-with-compound-toggle-arguments)
     *    [Restricting the set of values for an argument](#restricting-the-set-of-values-for-an-argument)
     *    [Using `option=value` syntax](#using-optionvalue-syntax)
     *    [Advanced usage formatting](#advanced-usage-formatting)
*    [Developer Notes](#developer-notes)
     *    [Copying and Moving](#copying-and-moving)
*    [CMake Integration](#cmake-integration)
*    [Building, Installing, and Testing](#building-installing-and-testing)
*    [Supported Toolchains](#supported-toolchains)
*    [Contributing](#contributing)
*    [License](#license)
-->

```cpp
#include "argparser.hpp"

int main(int argc, char **argv) {
   argparser::ArgumentParser parser("sample", "Python-style argparse for C++");
   parser.add_argument("input").help("input file");
   parser.add_argument("-v", "--verbose").action("store_true");
   parser.add_argument("-n", "--number").default_value(1);

   try {
      const auto& ns = parser.parse_args(argc, argv);
      auto input = ns.get<std::string>("input");
      auto verbose = ns.get<bool>("verbose");
      auto number = ns.get<int>("number");
      (void)input; (void)verbose; (void)number;
   } catch (const argparser::HelpError&) {
      parser.print_help();
   }

   return 0;
}
```

### Enhanced Help Formatter

```cpp
#include "argparser.hpp"

int main(int argc, char** argv) {
   argparser::ArgumentParser parser("mytool");
   parser.add_argument("-v", "--verbose").action("store_true").help("verbose output");
   parser.add_argument("input").help("input file");

   parser.set_help_format(argparser::HelpFormatter::OutputFormat::Colored)
            .set_help_width(88)
            .set_help_description("Process files with advanced help output")
            .add_example("mytool -v data.txt", "Run with verbose logging")
            .add_note("Use --help to view all options")
            .add_warning("Input file must exist")
            .set_prefix_info("MyTool CLI")
            .set_suffix_info("Docs: https://example.com/mytool");

   try {
      parser.parse_args(argc, argv);
   } catch (const argparser::HelpError&) {
      parser.print_help();
   }
}
```

Useful formatter controls:
- `set_help_format(...)` supports `Plain`, `Colored`, `Markdown`, and `Compact`
- `use_help_colors(...)`, `set_color_theme(...)`, and `set_help_alignment(...)`
- `configure_help(format, use_colors, width)` for one-call setup
- `get_formatter()` for direct advanced customization

### Subcommands + Known Args

```cpp
#include "argparser.hpp"

int main(int argc, char** argv) {
   argparser::ArgumentParser parser("tool");
   parser.add_argument("-v", "--verbose").action("store_true");

   auto& run = parser.add_subcommand("run", "run tasks");
   run.add_argument("path");
   run.add_argument("--mode").choices({"fast", "safe"}).default_value("fast");

   const auto& ns = parser.parse_known_args(argc, argv);
   for (const auto& unknown : parser.unknown_args()) {
      (void)unknown;
   }

   if (ns.has_subcommand() && ns.subcommand() == "run") {
      auto path = ns.subcommand_namespace().get<std::string>("path");
      auto mode = ns.subcommand_namespace().get<std::string>("mode");
      (void)path; (void)mode;
   }
}
```

### Environment Fallback

```cpp
argparser::ArgumentParser parser("app");
parser.add_argument("--port").env("APP_PORT").default_value(8080);
```

### Example Help

```sh
$ ./sample --help
Welcome to Argparser
Usage: ./sample file.txt -v [FILE] [-h | -v ] [--output FILE]  [options...]
            file.txt : Output path [required]
            -v : Verbose Output [default: false]

Options:
               -k : An implicit int parameter [implicit: "3", required]
       -a,--alpha : An optional float parameter with default value [default: 0.6]
        -b,--beta : An optional float parameter with std::optional return [default: none]
     -n,--numbers : An int vector, comma separated [required]
          --files : multiple arguments [required]
       -c,--color : An Enum input [allowed: <red, blue, green>, required]
     -v,--verbose : A flag to toggle verbose [implicit: "true", default: false]
           --help : print help [implicit: "true", default: false]
```

## 🧪 Testing

Configure and build with tests enabled:

```sh
cmake -S . -B build -DARGPARSER_BUILD_TESTS=ON
cmake --build build
```

Run tests:

```sh
ctest --test-dir build --output-on-failure
```

Build examples:

```sh
cmake -S . -B build -DARGPARSER_BUILD_EXAMPLES=ON
cmake --build build
```

## 🔰 Contributing

- **💬 [Join the Discussions](https://github.com/djoezeke/argparser/discussions)**: Share your insights, provide feedback, or ask questions.
- **🐛 [Report Issues](https://github.com/djoezeke/argparser/issues)**: Submit bugs found or log feature requests for the `argparser` project.
- **💡 [Submit Pull Requests](https://github.com/djoezeke/argparser/blob/main/CONTRIBUTING.md)**: Review open PRs, and submit your own PRs.

<details closed>
<summary>Contributing Guidelines</summary>

1. **Fork the Repository**: Start by forking the project repository to your github account.
2. **Clone Locally**: Clone the forked repository to your local machine using a git client.
   ```sh
   git clone --recursive https://github.com/djoezeke/argparser
   ```
3. **Create a New Branch**: Always work on a new branch, giving it a descriptive name.
   ```sh
   git checkout -b new-feature-x
   ```
4. **Make Your Changes**: Develop and test your changes locally.
5. **Commit Your Changes**: Commit with a clear message describing your updates.
   ```sh
   git commit -m 'Implemented new feature x.'
   ```
6. **Push to github**: Push the changes to your forked repository.
   ```sh
   git push origin new-feature-x
   ```
7. **Submit a Pull Request**: Create a PR against the original project repository. Clearly describe the changes and their motivations.
8. **Review**: Once your PR is reviewed and approved, it will be merged into the main branch. Congratulations on your contribution!
</details>

<details closed>
<summary>Contributor Graph</summary>
<br>
<p align="left">
   <a href="https://github.com{/djoezeke/argparser/}graphs/contributors">
      <img src="https://contrib.rocks/image?repo=djoezeke/argparser">
   </a>k
</p>
</details>

---

## 🙌 Acknowledgments

`Argparser` is inspired by Python Argparse and aims to provide a simple and lightweight solution for C/C++ projects.

### References

### More Reading

---

## 📃 License

This project is protected under the [MIT](LICENSE) License.
For more details, refer to the [LICENSE](LICENSE) file.
