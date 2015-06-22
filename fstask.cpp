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

namespace bf = boost::filesystem;
namespace bs = boost::system;

static const char PATTERN = '~';

void analyze_filesystem(bf::path root)
{
	auto exit_after_error = [](std::initializer_list<std::string> err_list)
	{
		std::cerr << "cleanup: error: ";
		std::copy(err_list.begin(), err_list.end(), 
		          std::ostream_iterator<std::string>(std::cerr));
		std::cerr << std::endl;
		std::exit(EXIT_FAILURE);
	};
	
	if(root.empty())
	{
		root = bf::path(getenv("HOME"));
		if(root.empty())
			exit_after_error({"no path specified and $HOME is not set"});
	}
	
	if(!bf::is_directory(root))
		exit_after_error({"'",std::string(root.c_str()),"' is not a directory"});
	
	fstask& root_task = *new (tbb::task::allocate_root()) fstask(std::move(root));
	tbb::task::spawn_root_and_wait(root_task);

}

tbb::task* fstask::execute()
{
	boost::system::error_code e;
	tbb::task_list t_list;
	
	// The ref count needs to be initialized with 1 as we wait in the end
	set_ref_count(1);

	// Iterate over subdirectories/files
	for(auto it = bf::directory_iterator(path_, e); 
		e == bs::errc::success && it != bf::directory_iterator(); ++it)
	{
		bf::path p = *it;

		if(bf::is_regular_file(p, e) && e == bs::errc::success)
		{
			trash_ptr->increment_file_count();
			
			const char* file_name = p.c_str(); 
			if(file_name[std::strlen(file_name)-1] == PATTERN)
				trash_ptr->add(p);
		}
		else if(bf::is_directory(p, e) && e == bs::errc::success)
		{
			fstask& child = *new(tbb::task::allocate_child()) fstask(std::move(p));
			increment_ref_count();
			t_list.push_back(child);
		}
	
		e = make_error_code(bs::errc::success);
	}
	
	if(!t_list.empty())
		spawn_and_wait_for_all(t_list);
	
	return nullptr;
}
