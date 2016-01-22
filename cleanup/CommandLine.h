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

#pragma once
#ifndef CLEANUP_COMMANDLINE_H
#define CLEANUP_COMMANDLINE_H

#include <cleanup/Config.h>
#include <boost/program_options.hpp>
#include <string>

CLEANUP_NAMESPACE_BEGIN

/// Handle command-line arguments 
class CommandLine
{
public:
    /// Setup the commands to be looked for
    CommandLine();

    /// Setup the commands to be looked for and call CommandLine::parse
    CommandLine(int argc, const char* const argv[]);

    /// Invoke the option parser (exit program if an error occurs)
    void parse(int argc, const char* const argv[]);

    /// Print version information nd exit the program with EXIT_SUCCESS(0) 
    NORETURN void printVersion() NOEXCEPT;

    /// Print help information and exit the program with EXIT_SUCCESS(0) 
    NORETURN void printHelp() const NOEXCEPT;

    /// Check whether the command-line argument @c arg has been passed
    bool has(const std::string& arg) const
    {
        return (variableMap_.count(arg) == 1);
    }

    /// Get command-line argument @c arg as type @c T
    template <typename T>
    T as(const std::string& arg) const
    {
        return variableMap_[arg].as<T>();
    }

private:
    boost::program_options::variables_map variableMap_;
    boost::program_options::options_description desc_;
    boost::program_options::positional_options_description posDesc_;
};

CLEANUP_NAMESPACE_END

#endif
