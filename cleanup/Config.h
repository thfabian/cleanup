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
#ifndef CLEANUP_CONFIG_H
#define CLEANUP_CONFIG_H

#define CLEANUP_VERSION_MAJOR 3
#define CLEANUP_VERSION_MINOR 0
#define CLEANUP_VERSION_STRING "3.0"

#define CLEANUP_NAMESPACE_BEGIN  namespace cleanup {
#define CLEANUP_NAMESPACE_END }

// NORETURN compiler intrinsic
#if defined(_MSC_VER)
#define NORETURN __declspec(noreturn)
#elif defined(__GNUC__)
#define NORETURN __attribute__((noreturn))
#else
#define NORETURN
#endif

// NOEXCEPT keyword
#if defined(__GNUC__) || _MSC_VER > 1800
#define NOEXCEPT noexcept
#else
#define NOEXCEPT throw()
#endif

// CONSTEXPR keyword
#if defined(__GNUC__) || _MSC_VER > 1800
#define CONSTEXPR constexpr
#else
#define CONSTEXPR const
#endif

// INLINE compiler intrinsic
#if defined(__GNUC__)
#define INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define INLINE inline __forceinline
#else
#define INLINE inline
#endif

// Define platform
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#define CLEANUP_PLATFORM_WINDOWS 1
#elif defined(__linux__ ) || defined(__linux)
#define CLEANUP_PLATFORM_LINUX 1
#elif defined(__APPLE__)
#define CLEANUP_PLATFORM_APPLE 1
#endif

#if defined (__unix__) || defined(CLEANUP_PLATFORM_APPLE)
#define CLEANUP_PLATFORM_POSIX 1
#endif

// Define compiler
#if defined(__clang__)
#define CLEANUP_COMPILER_CLANG 1
#endif

#if defined(__ICC) || defined(__INTEL_COMPILER)
#define CLEANUP_COMPILER_INTEL 1
#endif 

#if defined(__GNUC__) || defined(__GNUG__)
#define CLEANUP_COMPILER_GNU 1
#endif

#if defined(_MSC_VER)
#define CLEANUP_COMPILER_MSVC 1
#endif

// Exclude unnecessary APIs / Macros on Windows 
#ifdef CLEANUP_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX 1
#define _CRT_SECURE_NO_WARNINGS 1
#endif

// Ignore some annoying warnings
#ifdef CLEANUP_PLATFORM_WINDOWS
#pragma warning(disable : 4800) // forcing value to bool 'true' or 'false'
#endif

#endif
