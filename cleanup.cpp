/*******************************************************************- C++ -****\
 *                               
 *                           cleanup v2.0
 *                     (c) 2015 Fabian Thüring
 *
 * This file is distributed under the MIT Open Source License. See 
 * LICENSE.TXT for details.
 *
\******************************************************************************/

#include "fstask.hpp"

#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>  
#include <locale>
#include <exception>

namespace po = boost::program_options;

trash* trash_ptr;

/// Print 3 digits grouped (e.g 112305 as 112'305)
struct grouped : public std::numpunct<char>
{
	explicit grouped(std::size_t refs = 0) : std::numpunct<char>(refs) {}
    virtual char do_thousands_sep() const { return '\''; }
    virtual std::string do_grouping() const { return "\003"; }
};
 
/// Main entry point
int main(int argc, char* argv[])
{
	/// Command line options
	po::options_description desc("OPTIONS");

	desc.add_options()
		("help", "Display this information.")
		("version", "Display version information.")
		("cache,c", "Print the commands to run, but don't execute them.")
		("path,p", po::value< std::vector<std::string> >(),
		 "Specify the path at which the recursive search begins. By default the "
		 "environnement variable 'HOME' is being used.");

	// Parse
	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);  
	} 
	catch(std::exception& e)
	{
		std::cerr << "cleanup: error: " <<  e.what() << ", for help type" 
		          << " 'cleanup --help'" << std::endl;
		return 1;
	}

	// --help	
	if(vm.count("help")) 
	{
		std::cout << "cleanup v2.0";
#ifndef DEBUG
		std::cout << " - Release build";
#else
		std::cout << " - Debug build";
#endif

		std::cout << " [TBB]" << std::endl;

		std::cout << "\nUSAGE: cleanup [options]\n" << std::endl;
		std::cout << desc << std::endl;
		return 0;
	}

	// --version	
	if(vm.count("version")) 
	{
		std::cout << "cleanup version 2.0" << std::endl;
		return 0;
	}
	
	// --path
	std::string path = "";
	if(vm.count("path"))
	{
		path = vm["path"].as< std::vector<std::string> >()[0];
	}
	
	trash_ptr = new trash(vm.count("cache"));
	
	auto t_start = std::chrono::high_resolution_clock::now();
	
	/// Search file system
	analyze_filesystem(path);
	
	auto t_end = std::chrono::high_resolution_clock::now();
	
	/// Notify deleter thread
	trash_ptr->done();
	
	/// Print stats
	std::size_t cnt = trash_ptr->remove_counter();

	std::cout.imbue( std::locale(std::cout.getloc(), new grouped));
	std::cout << "cleanup: scanned " << trash_ptr->file_count() << " files "
	          << "in " << std::fixed << std::setprecision(0)
	          << std::chrono::duration<double, std::milli>(t_end - t_start).count()
	          << " ms -> " << (vm.count("cache") ? "found " : "deleted ") 
	          << cnt << (cnt == 1 ? " file" : " files") << std::endl;
	
	delete trash_ptr;
	return 0;
}
