/* single worker thread pool */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>

class WorkerThread {

private:
	std::thread worker;
	std::queue<std::function<void()>> taskQueue;
	std::mutex queueMutex;
	std::condition_variable conditionVar;
	std::atomic<bool> stopFlag;

	void threadFunc() {

		while (true) {
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(queueMutex);
				conditionVar.wait(lock, [this] {return stopFlag || !taskQueue.empty();});
				if (stopFlag && taskQueue.empty()) {
					break;
				}
				task = std::move(taskQueue.front());
				taskQueue.pop();
			}
			task(); // Execute the task
		}
	}

public:
	// Constructor
	WorkerThread() : stopFlag(false) {
		worker = std::thread(&WorkerThread::threadFunc, this); // Start a new thread that runs this->threadFunc()
	}

	// Destructor
	~WorkerThread() {
		{
			std::unique_lock<std::mutex> lock(queueMutex); 
			stopFlag = true; 
		}
		conditionVar.notify_one(); // Ensure the thread doesn't remain stuck on conditionVar.wait()
		worker.join(); // Wait for the worker thread to finish
	}

	// Add a task to the taskQueue
	void AddTask(std::function<void()> task) {
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			taskQueue.push(task);
		}
		conditionVar.notify_one();
	}
};

int main() {

	WorkerThread worker;

	std::this_thread::sleep_for(std::chrono::seconds(2));

	worker.AddTask([] { std::cout << ">>> Hello from Task 1\n"; });
	worker.AddTask([] { std::cout << ">>> Hello from Task 2\n"; });

	std::this_thread::sleep_for(std::chrono::seconds(5));

	return 0;
}

