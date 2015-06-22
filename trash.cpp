/*******************************************************************- C++ -****\
 *                               
 *                           cleanup v2.0
 *                     (c) 2015 Fabian Thüring
 *
 * This file is distributed under the MIT Open Source License. See 
 * LICENSE.TXT for details.
 *
\******************************************************************************/

#include "trash.hpp"

namespace bf = boost::filesystem;

trash::trash(bool cache)
	: file_count_(0), queue_(), remove_counter_(0), cache_(cache), done_(false)
{
	deleter_thread_ = std::thread(&trash::delete_file, this);
}

trash::~trash()
{
	deleter_thread_.join();
}

void trash::delete_file()
{
	bf::path p;
	
	std::unique_lock<std::mutex> lock(m_);
	while(!done_)
	{
		while(queue_.try_pop(p))
		{
			// Delete the file
			if(cache_)
			{
				std::cout << "> " << p << std::endl;
			}
			else if(std::remove(p.c_str()) != 0)
			{
				std::stringstream ss;
				ss << "cleanup: cannot remove " << p;
				std::perror(ss.str().c_str());
			}
			remove_counter_++;
		}
		
		cv_.wait(lock);
	}
}
