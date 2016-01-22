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
#ifndef CLEANUP_FILE_SYSTEM_BOOST_H
#define CLEANUP_FILE_SYSTEM_BOOST_H

#include <cleanup/Config.h>
#include <boost/filesystem.hpp>
#include <tbb/task.h>

CLEANUP_NAMESPACE_BEGIN

/// Boost based implementation of the FileSystem
class FileSystemBoost : tbb::task
{
    boost::filesystem::path path_;

public:
    /// Move Constructor
    FileSystemBoost(boost::filesystem::path&& curDir) : path_(curDir) {}

    /// Overrides virtual function task::execute
    tbb::task* execute() override;
    
    /// Analyze file system (root call)
    static void analyzeFilesystem(boost::filesystem::path root);
};

CLEANUP_NAMESPACE_END

#endif

