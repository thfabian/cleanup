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
#ifndef CLEANUP_CONFIG_HPP
#define CLEANUP_CONFIG_HPP

#define CLEANUP_VERSION_MAJOR 2
#define CLEANUP_VERSION_MINOR 1
#define CLEANUP_VERSION_STRING "2.1"

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
#if defined(__GNUC__)
#define NOEXCEPT noexcept
#elif defined(_MSC_VER)
#define NOEXCEPT throw()
#else
#define NOEXCEPT
#endif

// CONSTEXPR keyword
#if defined(__GNUC__)
#define CONSTEXPR constexpr
#elif defined(_MSC_VER)
#define CONSTEXPR const
#else
#define CONSTEXPR const
#endif

#endif
