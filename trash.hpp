/*******************************************************************- C++ -****\
 *                               
 *                           cleanup v2.0
 *                     (c) 2015 Fabian Thüring
 *
 * This file is distributed under the MIT Open Source License. See 
 * LICENSE.TXT for details.
 *
\******************************************************************************/

#ifndef TRASH_HPP
#define TRASH_HPP

#include <tbb/concurrent_queue.h>
#include <boost/filesystem.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdio>
#include <iostream>
#include <sstream>

class trash
{
private:
	std::atomic<int> file_count_;
	tbb::concurrent_queue<boost::filesystem::path> queue_;
	std::size_t remove_counter_;
	bool cache_;
	
	bool done_;
	std::mutex m_;
	std::condition_variable cv_;
	std::thread deleter_thread_;
	
public: 

	/// Constructor - launch deleter thread
	trash(bool cache);
	
	/// Constructor - join deleter thread
	~trash();

	/// Increment file counter
	inline void increment_file_count() { file_count_++; }

	/// Add a file to the queue
	inline void add(const boost::filesystem::path& p) 
	{ 
		queue_.push(p); 
		cv_.notify_all();
	}
	
	/// Wake up deleter thread and terminate execution
	inline void done()
	{
		std::unique_lock<std::mutex> lock(m_);
		done_ = true;
		cv_.notify_all();
	}
	
	/// Delete all elements in the queue until queue is empty and go to sleep 
	/// at condition variable cv_ afterwards
	void delete_file();
	
	inline std::size_t remove_counter() const { return remove_counter_; }
	inline int file_count() const { return file_count_; }
};

/// Forward definition of the trash_ptr (allocated in cleanup.cpp)
extern trash* trash_ptr;

#endif // TRASH_HPP
