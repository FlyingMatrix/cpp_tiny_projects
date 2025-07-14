#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>
#include <vector>
#include <sstream> 


class ThreadPool {
private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> taskQueue;
	std::mutex queueMutex;
	std::mutex coutMutex;
	std::condition_variable conditionVar;
	std::atomic<bool> stopFlag;

	void threadFunc() {
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cout << ">>> Worker thread started with ID: " << std::this_thread::get_id() << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		while (true) {
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(queueMutex);
				conditionVar.wait(lock, [this] { return stopFlag || !taskQueue.empty(); });
				if (stopFlag && taskQueue.empty()) {
					break;
				}
				task = std::move(taskQueue.front());
				taskQueue.pop();
			}
			task(); // Execute task
		}
	}

public:
	// Constructor
	ThreadPool(size_t numThreads) : stopFlag(false) {
		for (size_t i = 0; numThreads > i; ++i) {
			threads.emplace_back(&ThreadPool::threadFunc, this);
		}
	}

	// Destructor
	~ThreadPool() {
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			stopFlag = true;
		}
		conditionVar.notify_all();
		for (std::thread& thread : threads) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}

	// Add a task
	void AddTask(std::function<void()> task) {
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			taskQueue.push(std::move(task));
		}
		conditionVar.notify_one();
	}

	// Thread-safe output method
	void PrintThreadSafe(const std::string& message) {
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << message << std::endl;
	}
};


int main() {
	const size_t numThreads = 4;
	ThreadPool pool(numThreads);

	const size_t numTasks = 20;
	for (size_t i = 1; numTasks >= i; ++i) {
		pool.AddTask([i, &pool] {
			std::ostringstream oss;
			oss << ">>> Task " << i << " is running on thread " << std::this_thread::get_id();
			pool.PrintThreadSafe(oss.str());
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			});
	}

	// Let threads finish processing
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 0;
}

