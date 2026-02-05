/*
* DISCLAIMER main code is from geeksforgeeks.org
* https://www.geeksforgeeks.org/dsa/implement-thread-safe-queue-in-c/
*/
#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T>
class TSQueue
{
public:
	/*
	* Locks the queue then pushes an object into the queue
	*/
	inline void Push(T item)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		m_queue.push(std::move(item));

		//notfiy a thread that is waiting
		m_cond.notify_one();
	}

	/*
	* Locks the queue then pops an object from the queue
	*/
	inline T Pop()
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		m_cond.wait(lock, [this]() {return !m_queue.empty(); });

		T item = std::move(m_queue.front());

		m_queue.pop();

		return item;
	}

	inline bool TryPop(T& out)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_queue.empty()) return false;

		out = std::move(m_queue.front());

		m_queue.pop();

		return true;
	}

private:
	std::queue<T> m_queue;
	//mutex for thread synchronization
	std::mutex m_mutex;
	//conditional variable for signaling
	std::condition_variable m_cond;
};

