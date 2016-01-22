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
#ifndef CLEANUP_SEARCH_PATHS_H
#define CLEANUP_SEARCH_PATHS_H

#include <cleanup/Config.h>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

CLEANUP_NAMESPACE_BEGIN

namespace internal 
{

/// Paths which will be serialized
class Paths
{
public:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* version */)
    {
        ar & BOOST_SERIALIZATION_NVP(paths);
    }

    std::vector<std::string> paths;
};

/// Get value of environment variable HOME (Unix) or HOMEPATH (Windows)
#ifdef CLEANUP_PLATFORM_WINDOWS
wchar_t* getEnvHome();
#else
char* getEnvHome();
#endif

}

/// Configure the search paths
class SearchPaths
{
    internal::Paths paths_;
    
    boost::filesystem::path filePath_;
    bool updateConfigFile_;

public:
    /// Extract paths from XML file (either given directly as an argument or 
    /// look in $HOME/.cleanup/config.xml on Unix or %HOMEPATH%/.cleanup/config
    /// on Windows).
    SearchPaths(const std::string& file);

    ~SearchPaths();

    /// Write changes to config file
    void updateConfigFile();

    /// Return vector of search paths (potentially empty)
    const std::vector<std::string>& getPaths() const NOEXCEPT
    {
        return paths_.paths;
    }

    /// Add a path
    void addPath(const std::string& path) NOEXCEPT
    {
        updateConfigFile_ = true;
        paths_.paths.push_back(path);
    }

    /// Remove a path
    void removePath(const std::string& path) NOEXCEPT
    {
        auto it = std::find(paths_.paths.begin(), paths_.paths.end(), path);
        if(it != paths_.paths.end())
        {
            paths_.paths.erase(it);
            updateConfigFile_ = true;
        }
        else
            Error::fatal({"cannot remove non-existent directory '", path, "'"});
    }
};

CLEANUP_NAMESPACE_END

BOOST_CLASS_VERSION(cleanup::internal::Paths, 0);

#endif
