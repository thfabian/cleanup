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

#include <cleanup/RemoveFile.h>
#include <cstdio>
#include <iostream>
#include <sstream>

CLEANUP_NAMESPACE_BEGIN

namespace bf = boost::filesystem;

RemoveFile::RemoveFile(bool cache)
    : fileCount_(0), queue_(), removeCounter_(0), cache_(cache), done_(false)
{
    deleterThread_ = std::thread(&RemoveFile::deleteFile, this);
}

RemoveFile::~RemoveFile()
{
    deleterThread_.join();
}

void RemoveFile::deleteFile()
{
    bf::path p;

    std::unique_lock<std::mutex> lock(m_);
    while(!done_)
    {
        while(queue_.try_pop(p))
        {
            // Delete the file(s)
            if(cache_)
                std::cout << "> " << p << std::endl;
            else if(std::remove(p.string().c_str()) != 0)
            {
                std::stringstream ss;
                ss << "cleanup: cannot remove " << p;
                std::perror(ss.str().c_str());
            }
            removeCounter_++;
        }
        cv_.wait(lock);
    }
}

CLEANUP_NAMESPACE_END
