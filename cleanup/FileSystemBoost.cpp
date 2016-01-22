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

#include <cleanup/Error.h>
#include <cleanup/FileSystemBoost.h>
#include <cleanup/RemoveFile.h>
#include <boost/system/error_code.hpp>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <initializer_list>
#include <algorithm>

#ifdef CLEANUP_PLATFORM_WINDOWS
#define STRLEN(str) std::wcslen(str)
#else
#define STRLEN(str) std::strlen(str)
#endif

namespace fs = boost::filesystem;
namespace bs = boost::system;

CLEANUP_NAMESPACE_BEGIN

static const char PATTERN = '~';

void FileSystemBoost::analyzeFilesystem(fs::path rootDir)
{
    if(!fs::is_directory(rootDir))
        Error::fatal({"'", rootDir.string(), "' is not a directory"});

    FileSystemBoost& root_task
        = *new(tbb::task::allocate_root()) FileSystemBoost(std::move(rootDir));
    tbb::task::spawn_root_and_wait(root_task);
}

tbb::task* FileSystemBoost::execute()
{
    using CharType = fs::path::value_type;

    boost::system::error_code e;
    tbb::task_list t_list;

    // The ref count needs to be initialized with 1 as we wait in the end
    set_ref_count(1);

    // Iterate over subdirectories/files
    for(auto it = fs::directory_iterator(path_, e);
        e == bs::errc::success && it != fs::directory_iterator(); ++it)
    {

        fs::path p = *it;

        if(fs::is_regular_file(p, e) && e == bs::errc::success)
        {
            remove_ptr->incrementFileCount();
            const CharType* filename = p.c_str();
            if(filename[STRLEN(filename) - 1] == PATTERN)
                remove_ptr->add(std::move(p));
        }
        else if(fs::is_directory(p, e) && e == bs::errc::success)
        {
            FileSystemBoost& child = *new(tbb::task::allocate_child())
                                         FileSystemBoost(std::move(p));
            increment_ref_count();
            t_list.push_back(child);
        }

        e = bs::errc::make_error_code(bs::errc::success);
    }

    if(!t_list.empty())
        spawn_and_wait_for_all(t_list);


    return nullptr;
}

CLEANUP_NAMESPACE_END
