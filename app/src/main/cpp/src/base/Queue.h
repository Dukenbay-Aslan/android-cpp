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
    const std::size_t& size() const;
    bool empty();
  private:
    /**
     * @brief Underlying queue of items
     */
    std::queue<TItem> queue;
    /**
     * @brief Maximum number of items to store
     */
    std::size_t maxSize;

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
        return (queue.size() < maxSize);
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

/**
 * Get the current size of a queue
 * @return Size of an underlying queue
 */
template<class TItem>
const std::size_t& TQueue<TItem>::size() const {
    return queue.size();
}

/**
 * @brief Whether a queue is empty
 * @return `empty()` method of an underlying queue
 */
template<class TItem>
bool TQueue<TItem>::empty() {
    return queue.empty();
}