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

#include <cleanup/Reporter.h>
#include <iomanip>
#include <locale>
#include <iostream>

CLEANUP_NAMESPACE_BEGIN

/// Print 3 digits grouped (e.g 112305 as 112'305)
struct grouped : public std::numpunct<char>
{
    explicit grouped(std::size_t refs = 0) : std::numpunct<char>(refs) {}
    virtual char do_thousands_sep() const { return '\''; }
    virtual std::string do_grouping() const { return "\003"; }
};

void Reporter::print(bool cache)
{
    std::cout.imbue(std::locale(std::cout.getloc(), new grouped));
    std::cout << "cleanup: scanned " << fileCount_ << " files in " << std::fixed
              << std::setprecision(0) << elapsedTime_ << " ms -> "
              << (cache ? "found " : "deleted ") << removedFileCount_
              << (removedFileCount_ == 1 ? " file" : " files") << std::endl;
}


CLEANUP_NAMESPACE_END

