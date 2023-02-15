#pragma once
#include "../Macro.h"
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
    SINGLE_TONE(ThreadPool)

	void AddJob(std::function<void()> job);
private:
	void run();

private:
	std::vector<std::thread> mWorkerThreads;
	std::queue<std::function<void()>> mJobs;

	std::condition_variable mJobCV;
	std::mutex mJobMtx;
	bool mStop;
};

