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
#include <cleanup/SearchPaths.h>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <fstream>

namespace fs = boost::filesystem;

CLEANUP_NAMESPACE_BEGIN

#ifdef CLEANUP_PLATFORM_WINDOWS
wchar_t* cleanup::internal::getEnvHome()
{
    auto ptr = _wgetenv(L"HOMEPATH");
    if(!ptr)
        Error::fatal(
            {"Environment variable %HOMEPATH% does not exit or is empty"});
    return ptr;
}
#else
char* cleanup::internal::getEnvHome()
{
    auto ptr =std::getenv("HOME");
    if(!ptr)
        Error::fatal(
            {"Environment variable $HOME does not exit or is empty"});
    return ptr;

}
#endif

SearchPaths::SearchPaths(const std::string& file)
    :   updateConfigFile_(false)
{
    if(!file.empty())
    {
        filePath_ = file;

        if(!fs::is_regular_file(file))
            Error::fatal({"no such file or directory: '", file, "'"});
    }
    else
    {

        filePath_ = internal::getEnvHome();
        filePath_ /= ".cleanup";
        filePath_ /= "config.xml";
    }

    if(!fs::is_regular_file(filePath_))
        return;

    // Read configuration file
    std::ifstream fin(filePath_.string());
    if(!fin.good())
        Error::fatal(
            {"failed to open configuration file: ", filePath_.string()});

    boost::archive::xml_iarchive ia(fin);
    ia >> boost::serialization::make_nvp("Paths", paths_);

    // Validate
    for(auto it = paths_.paths.begin(); it != paths_.paths.end();)
    {
        if(!fs::is_directory(*it))
        {
            updateConfigFile_ = true;
            Error::warning({"'", *it, "' is not a directory"});
            it = paths_.paths.erase(it);
        }
        else
            ++it;
    }
}

SearchPaths::~SearchPaths()
{
    updateConfigFile();
}

void SearchPaths::updateConfigFile()
{
    if(!updateConfigFile_)
        return;

    boost::system::error_code e;
    fs::path cleanupDir = filePath_.parent_path();
    if(!fs::is_directory(cleanupDir) && fs::create_directory(cleanupDir, e))
        Error::fatal({"failed to create configuration directory '",
                      cleanupDir.string(), "': ", e.message()});

     std::ofstream fout(filePath_.string());
     if(!fout.good())
        Error::fatal(
            {"failed to open configuration file: ", filePath_.string()});

     boost::archive::xml_oarchive oa(fout);
     oa << boost::serialization::make_nvp("Paths", paths_);
    
}

CLEANUP_NAMESPACE_END

