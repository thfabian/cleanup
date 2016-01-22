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
#ifndef CLEANUP_REPORTER_H
#define CLEANUP_REPORTER_H

#include <cleanup/Config.h>
#include <cstddef>

CLEANUP_NAMESPACE_BEGIN

class Reporter
{
    std::size_t fileCount_;        ///< Total number of scanned files
    std::size_t removedFileCount_; ///< Removed or found files
    double elapsedTime_;           ///< Elapsed time

public:
    /// Initialize Reporter
    Reporter(std::size_t fileCount,
             std::size_t removedFileCount,
             double elapsedTime)
        : fileCount_(fileCount),
          removedFileCount_(removedFileCount),
          elapsedTime_(elapsedTime)
    {}

    /// Print report
    void print(bool cache);
};

CLEANUP_NAMESPACE_END

#endif
