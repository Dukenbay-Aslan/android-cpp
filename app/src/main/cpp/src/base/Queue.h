#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

/**
 * @brief Queue with waiting technique
 * @tparam TItem 
 */
template<class TItem>
class TQueue {
  public:
    void push(const TItem& item);
    TItem pop();
  private:
    /**
     * @brief Underlying queue of items
     */
    std::queue<TItem> queue;
    /**
     * @brief Maximum number of items to store
     */
    unsigned long long size;

    /**
     * @brief Mutual exlusion for `TQueue::conditionVariable`
     * @note Can be modified in `const` methods
     */
    mutable std::mutex mutex;
    /**
     * @brief Variable to wait for some condition to become true
     */
    std::condition_variable conditionVariable;
};

/**
 * @brief Push an item to the queue
 * @tparam TItem 
 * @param item Item to push
 * @warning Blocks until there is a space in the queue
 */
template<class TItem>
void TQueue<TItem>::push(const TItem& item) {
    std::unique_lock<std::mutex> lock(mutex);
    // Wait until there is a space in the queue
    conditionVariable.wait(lock, [this] {
        return (queue.size() < size);
    });
    queue.push(item);
    lock.unlock();
    conditionVariable.notify_one();
}

/**
 * @brief Pop an item 
 * @tparam TItem 
 * @return Item in the front of the queue. First In First Out (FIFO)
 * @warning Blocks until there is an item in the queue
 */
template<class TItem>
TItem TQueue<TItem>::pop() {
    std::unique_lock<std::mutex> lock(mutex);
    // Wait until there is an item in the queue
    conditionVariable.wait(lock, [this] {
        return (!queue.empty());
    });
	TItem item = queue.front();
	queue.pop();
    lock.unlock();
    conditionVariable.notify_one();
	return item;
}