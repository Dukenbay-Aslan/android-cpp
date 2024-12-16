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
    TQueue();
    TQueue(unsigned long long maxSize);
    void push(const TItem& item);
    void push(TItem&& item);
    TItem pop();
    const std::size_t& maxSize() const;
    std::size_t size();
    bool empty();
  private:
    /**
     * @brief Underlying queue of items
     */
    std::queue<TItem> queue;
    /**
     * @brief Maximum number of items to store
     * Defaults to 64
     */
    std::size_t maxSize_ = 64;

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
 * @brief Default constructor.
 * @tparam TItem Type of items
 * `TQueue<TItem>::maxSize_ = 64`
 */
template<class TItem>
TQueue<TItem>::TQueue()
    : maxSize_(64)
{

}

/**
 * @brief Constructor
 * @tparam TItem Type of items
 * @param maxSize Maximum number of items to store
 */
template<class TItem>
TQueue<TItem>::TQueue(unsigned long long maxSize)
    : maxSize_(maxSize)
{

}

/**
 * @brief Push an item to the queue
 * @tparam TItem Type of items
 * @param item Item to push
 * @warning Blocks until there is space in the queue
 */
template<class TItem>
void TQueue<TItem>::push(const TItem& item) {
    std::unique_lock<std::mutex> lock(mutex);
    // Wait until there is a space in the queue
    conditionVariable.wait(lock, [this] {
        return (queue.size() < maxSize_);
    });
    queue.push(item);
    lock.unlock();
    conditionVariable.notify_one();
}

/**
 * @brief Push a move-only item to the queue
 * @tparam TItem Type of items
 * @param item Item to push
 * @warning Blocks until there is space in the queue
 */
template<class TItem>
void TQueue<TItem>::push(TItem&& item) {
    std::unique_lock<std::mutex> lock(mutex);
    // Wait until there is a space in the queue
    conditionVariable.wait(lock, [this] {
        return (queue.size() < maxSize_);
    });
    queue.push(std::move(item));
    lock.unlock();
    conditionVariable.notify_one();
}

/**
 * @brief Pop an item 
 * @tparam TItem Type of items
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
	TItem item = std::move(queue.front());
	queue.pop();
    lock.unlock();
    conditionVariable.notify_one();
	return std::move(item);
}

/**
 * @brief Get the current size of a queue
 * @tparam TItem Type of items
 * @return Size of an underlying queue
 */
template<class TItem>
std::size_t TQueue<TItem>::size() {
    return queue.size();
}

/**
 * @brief Get the capacity of a queue
 * @tparam TItem Type of items
 * @return `TQueue<TItem>::maxSize_`
 */
template<class TItem>
const std::size_t& TQueue<TItem>::maxSize() const {
    return maxSize_;
}

/**
 * @brief Whether a queue is empty
 * @tparam TItem Type of items
 * @return `empty()` method of an underlying queue
 */
template<class TItem>
bool TQueue<TItem>::empty() {
    return queue.empty();
}