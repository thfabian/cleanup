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
#ifndef CLEANUP_REMOVE_FILE_H
#define CLEANUP_REMOVE_FILE_H

#include <cleanup/Config.h>
#include <boost/filesystem.hpp>
#include <tbb/concurrent_queue.h>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

CLEANUP_NAMESPACE_BEGIN

class RemoveFile
{
    std::atomic<int> fileCount_;
    tbb::concurrent_queue<boost::filesystem::path> queue_;
    std::size_t removeCounter_;
    bool cache_;

    bool done_;
    std::mutex m_;
    std::condition_variable cv_;
    std::thread deleterThread_;

public:
    /// Constructor - launch deleter thread
    RemoveFile(bool cache);

    /// Destructor - join deleter thread
    ~RemoveFile();

    /// Increment file counter
    INLINE void incrementFileCount() { fileCount_++; }

    /// Add a file to the queue
    INLINE void add(const boost::filesystem::path& p)
    {
        queue_.push(p);
        cv_.notify_all();
    }

    /// Wake up deleter thread and terminate execution
    inline void done()
    {
        std::unique_lock<std::mutex> lock(m_);
        done_ = true;
        cv_.notify_all();
    }

    /// Delete all elements in the queue until queue is empty and go to sleep
    /// at condition variable cv_ afterwards
    void deleteFile();

    inline std::size_t removeCounter() const { return removeCounter_; }
    inline std::size_t fileCount() const { return std::size_t(fileCount_); }
};

/// Forward definition of the trash_ptr (allocated in main.cpp)
extern RemoveFile* remove_ptr;

CLEANUP_NAMESPACE_END

#endif

