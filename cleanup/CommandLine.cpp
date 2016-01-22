/**               _______   _______   _  ____  _____ 
 *               / ___/ /  / __/ _ | / |/ / / / / _ \
 *              / /__/ /__/ _// __ |/    / /_/ / ___/
 *              \___/____/___/_/ |_/_/|_/\____/_/    
 *                                        
 *   Copyright (C) 2016  Fabian Thuering (fabian_thuering@hotmail.com)
 *
 * This file is distributed under the MIT Open Source License. See
 * LICENSE.TXT for details.
 */

#include <cleanup/CommandLine.h>
#include <cleanup/Error.h>
#include <boost/filesystem.hpp>
#include <boost/version.hpp>
#include <tbb/tbb_stddef.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <initializer_list>
#include <string>
#include <vector>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

CLEANUP_NAMESPACE_BEGIN

CommandLine::CommandLine() : desc_("Options")
{
    desc_.add_options()
        // --help
        ("help", "Display this information.")
        // --version
        ("version", "Display version information.")
        // --cache, -c
        ("cache,c", "Print the found files but don't delete them.")
        // --quiet
        ("quiet", "Be quit (no output to terminal).")
        // --single
        ("single", "Run single threaded version.")
        // --dir, -d
        ("dir,d", po::value<std::vector<std::string>>(),
         "Specify the directories at which the recursive search shall start. "
         "These directories will be merged with the directories found in the "
         "configuration file.")
        // --config-file
        ("config-file", po::value<std::vector<std::string>>(),
         "Absolute path to the configuration file. If nothing is specified, "
         "the program will use:"
#ifdef CLEANUP_PLATFORM_WINDOWS
         "\n %HOMEPATH%\\.cleanup\\config.xml")
#else
         "\n $HOME/.cleanup/config.xml")
#endif
        // --add-dir
        ("add-dir", po::value<std::vector<std::string>>(),
         "Add a directory to the config file.")
        // --remove-dir
        ("remove-dir", po::value<std::vector<std::string>>(),
         "Remove a directory from the config file.");

    // Associate positional arguments to options
    posDesc_.add("dir", -1);
}

CommandLine::CommandLine(int argc, const char* const argv[]) : CommandLine()
{
    parse(argc, argv);
}

/// Check if the value of arg is in the list
template <class T, class VarMap>
inline void validate(const std::string& arg,
                     const VarMap& varMap,
                     std::initializer_list<T> list)
{
    if(varMap.count(arg)
       && std::count(list.begin(), list.end(), varMap[arg].template as<T>())
              == 0)
        throw po::validation_error(po::validation_error::invalid_option_value,
                                   arg, varMap[arg].template as<T>());
}

void CommandLine::parse(int argc, const char* const argv[])
{
    try
    {
        po::store(po::command_line_parser(argc, argv)
                      .options(desc_)
                      .positional(posDesc_)
                      .run(),
                  variableMap_);
        po::notify(variableMap_);
    }
    catch(const std::exception& e)
    {
        Error::fatal({e.what(), ", for help type '",
                      fs::path(argv[0]).filename().string(), " --help'"});
    }
}

NORETURN void CommandLine::printHelp() const NOEXCEPT
{
    std::cout << "Usage: cleanup [options] directories...\n\n";
    std::cout << desc_ << std::endl;
    std::exit(EXIT_SUCCESS);
}

void CommandLine::printVersion() NOEXCEPT
{
    std::cout << "cleanup - " << CLEANUP_VERSION_STRING
        << "\nCopyright (C) 2016 Fabian Thuering. All rights reserved."
        << " There is NO warranty; \nnot even for MERCHANTABILITY or FITNESS "
        << "FOR A PARTICULAR PURPOSE.\n";
        
    // Tag compiler
#if defined(CLEANUP_COMPILER_MSVC)
    std::cout << " - Compiler: Microsoft(R) Visual C/C++ " << _MSC_VER << "\n";
#elif defined(CLEANUP_COMPILER_INTEL)
    std::cout << " - Compiler: " << __VERSION__ << "\n";
#elif defined(CLEANUP_COMPILER_CLANG)
    std::cout << " - Compiler: LLVM Clang " << __clang_version__ << "\n";
#elif defined(CLEANUP_COMPILER_GNU)
    std::cout << " - Compiler: GNU GCC/G++ " << __VERSION__ << "\n";
#endif

    // Boost version
    std::cout << " - Boost version: " << BOOST_LIB_VERSION << "\n";

    // Boost version
    std::cout << " - TBB version: " << TBB_VERSION_MAJOR << "."
              << TBB_VERSION_MINOR << "\n";

    std::exit(EXIT_SUCCESS);
}

CLEANUP_NAMESPACE_END

