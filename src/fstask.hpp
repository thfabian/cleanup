/*******************************************************************- C++ -****\
 *
 *                           cleanup v2.0
 *                     (c) 2015 Fabian Thüring
 *
 * This file is distributed under the MIT Open Source License. See
 * LICENSE.TXT for details.
 *
\******************************************************************************/

#pragma once
#ifndef CLEANUP_FS_TASK_HPP
#define CLEANUP_FS_TASK_HPP

#include "config.hpp"
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <tbb/task.h>

CLEANUP_NAMESPACE_BEGIN

/// TBB Task implemenation to recursively traverse the file system
class fstask : public tbb::task
{
private:
    boost::filesystem::path path_;

public:
    /// Move Constructor
    fstask(boost::filesystem::path&& cur_dir) : path_(cur_dir) {}

    /// Overrides virtual function task::execute
    tbb::task* execute() override;
    
    /// Analyze file system (root call)
    static void analyze_filesystem(boost::filesystem::path root);
};

CLEANUP_NAMESPACE_END

#endif /* fstask.hpp */

