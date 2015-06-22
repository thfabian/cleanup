/*******************************************************************- C++ -****\
 *                               
 *                           cleanup v2.0
 *                     (c) 2015 Fabian Thüring
 *
 * This file is distributed under the MIT Open Source License. See 
 * LICENSE.TXT for details.
 *
\******************************************************************************/

#ifndef FS_TASK_HPP
#define FS_TASK_HPP

#include "trash.hpp"

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp> 
#include <tbb/task.h>
 
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <initializer_list>
#include <algorithm>

/// Analyze file system (root call)
void analyze_filesystem(boost::filesystem::path root);

/// TBB Task implemenation to recursively traverse the file system
class fstask : public tbb::task 
{
private:
	boost::filesystem::path path_;

public:
    /// Move Constructor
    fstask(boost::filesystem::path&& cur_dir)
    	:	path_(cur_dir)
    {}

    /// Overrides virtual function task::execute
    tbb::task* execute() override; 
};


#endif /* fstask.hpp */
