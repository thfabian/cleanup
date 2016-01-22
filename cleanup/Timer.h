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
#ifndef CLEANUP_TIMER_H
#define CLEANUP_TIMER_H

#include <cleanup/Config.h>
#include <chrono>

CLEANUP_NAMESPACE_BEGIN

/// High resolution timer
class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;

public:
    /// Start the timer
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    /// Reset the timer to the current time
    inline void start() NOEXCEPT
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    /// Return the number of milliseconds elapsed since the timer was last reset
    inline double stop() NOEXCEPT
    {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }
};

CLEANUP_NAMESPACE_END

#endif

