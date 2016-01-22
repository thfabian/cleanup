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

#include <cleanup/CommandLine.h>
#include <cleanup/Error.h>
#include <cleanup/FileSystemBoost.h>
#include <cleanup/RemoveFile.h>
#include <cleanup/Reporter.h>
#include <cleanup/SearchPaths.h>
#include <cleanup/Timer.h>
#include <tbb/task_scheduler_init.h>
#include <string>
#include <vector>

using namespace cleanup;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

std::string Error::Program = "cleanup";
RemoveFile* cleanup::remove_ptr;

/// Main entry point
int main(int argc, char* argv[])
{
    Error::setProgramName(argv[0]);
    CommandLine cmd(argc, argv);

    if(cmd.has("help"))
        cmd.printHelp();

    if(cmd.has("version"))
        cmd.printVersion();

    tbb::task_scheduler_init init(
        cmd.has("single") ? 1 : tbb::task_scheduler_init::automatic);

    // ---- Setup paths ----
    std::vector<std::string> paths;

    // 1. Read config file
    SearchPaths searchPaths(
        cmd.has("config-file")
            ? cmd.as<std::vector<std::string>>("config-file")[0]
            : "");

    if(cmd.has("add-dir"))
        searchPaths.addPath(cmd.as<std::vector<std::string>>("add-dir")[0]);

    if(cmd.has("remove-dir"))
        searchPaths.removePath(cmd.as<std::vector<std::string>>("remove-dir")[0]);

    searchPaths.updateConfigFile();

    // 2. Parse command-line arguments
    if(cmd.has("dir"))
        paths = cmd.as<std::vector<std::string>>("dir");

    paths.insert(paths.end(), searchPaths.getPaths().cbegin(),
                 searchPaths.getPaths().cend());

    // 3. Nothing is specified fall-back to $HOME
    if(paths.empty())
        paths.push_back(fs::path(internal::getEnvHome()).string());

    // ---- Analyze filesystem ----
    remove_ptr = new RemoveFile(cmd.has("cache"));

    Timer t;
    for(const auto& path : paths)
        FileSystemBoost::analyzeFilesystem(path);
    
    auto time = t.stop();
    remove_ptr->done();

    // ---- Report results ----
    Reporter reporter(remove_ptr->fileCount(), remove_ptr->removeCounter(),
                      time);

    if(!cmd.has("quiet"))
        reporter.print(cmd.has("cache"));

    delete remove_ptr;
    return 0;
}
