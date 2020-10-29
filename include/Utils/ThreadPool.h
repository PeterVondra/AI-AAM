#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <future>

namespace exz
{
	namespace utils
	{
		class ThreadPool
		{
			public:
				ThreadPool(const unsigned int p_NumThreads);




			private:

				const unsigned int m_NumThreads;

				std::mutex m;
				std::condition_variable v;
				std::deque<std::packaged_task<void()>> work;

				std::vector<std::future<void>> finished;


		};
	}
}