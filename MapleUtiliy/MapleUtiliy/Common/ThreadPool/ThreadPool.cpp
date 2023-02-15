#include "ThreadPool.h"

INIT_INSTACNE(ThreadPool)
ThreadPool::ThreadPool()
	: mStop(false)
{
    size_t num = std::thread::hardware_concurrency() - 1;
	mWorkerThreads.reserve(num);

	for (size_t i = 0; i < num; ++i)
	{
		mWorkerThreads.emplace_back(std::thread{&ThreadPool::run, this});
	}
}

ThreadPool::~ThreadPool()
{
	mStop = true;
	mJobCV.notify_all();

	for (auto& th : mWorkerThreads)
	{
        if(th.joinable() == true)
        {
            th.join();
        }
	}
}

void ThreadPool::AddJob(std::function<void()> job)
{
	if (mStop == true)
	{
		return;
	}

	mJobMtx.lock();
	mJobs.push(job);
	mJobMtx.unlock();

	mJobCV.notify_one();
}

void ThreadPool::run()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mJobMtx);
		mJobCV.wait(lock, [this]() { return mJobs.empty() == false || mStop == true; });

		// 처리할 일이 없고, 스탑 명령일 때,
		if (mJobs.empty() == true && mStop == true)
		{
			return;
		}
		
		auto job = mJobs.front();
		mJobs.pop();
		lock.unlock();

		job();
	}
}
