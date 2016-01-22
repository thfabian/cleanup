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
#ifndef CLEANUP_ERROR_H
#define CLEANUP_ERROR_H

#include <cleanup/Config.h>
#include <boost/filesystem.hpp>
#include <string>
#include <initializer_list>
#include <iostream>

CLEANUP_NAMESPACE_BEGIN

struct Error
{
    /// Set the program name (usually argv[0])
    static void setProgramName(const char* program) NOEXCEPT
    {
        Error::Program = boost::filesystem::path(program).filename().string();
    }

    /// Print an error message and exit the program with EXIT_FAILURE(1)
    NORETURN static void fatal(std::initializer_list<std::string> msg) NOEXCEPT
    {
        std::cerr << Program << ": error: ";
        for(auto str : msg)
            std::cerr << str;
        std::cerr << std::endl;
        std::exit(EXIT_FAILURE);
    }

    /// Print a warning message
    static void warning(std::initializer_list<std::string> msg) NOEXCEPT
    {
        std::cerr << Program << ": warning: ";
        for(auto str : msg)
            std::cerr << str;
        std::cerr << std::endl;
    }

    /// Name of the program (defined in main.cpp)
    static std::string Program;
};

CLEANUP_NAMESPACE_END

#endif