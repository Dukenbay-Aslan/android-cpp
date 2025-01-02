#pragma once

#include "ws.h"

namespace Ws {

/**
 * @brief Enhanced map with
 * storing, erasing and
 * swapping between two values
 * @tparam Key Key type
 * @tparam Value Value type
 */
template<class Key, class Value>
class TwoMap {
  public:
    Value& first(const Key& key);
    Value& second(const Key& key);
    void clear(const Key& key);
    void swap(const Key& key);
  private:
    /**
     * @brief First values
     */
    std::unordered_map<
        Key, Value> first_;
    /**
     * @brief Second values
     */
    std::unordered_map<
        Key, Value> second_;
};

/**
 * @brief Return the first value
 * @tparam Key Key type
 * @tparam Value Value type
 * @param key Key
 * @return First value
 */
template<class Key, class Value>
Value& TwoMap<Key, Value>::first(const Key& key) {
    return first_[key];
}

/**
 * @brief Return the second value
 * @tparam Key Key type
 * @tparam Value Value type
 * @param key Key
 * @return Second value
 */
template<class Key, class Value>
Value& TwoMap<Key, Value>::second(const Key& key) {
    return second_[key];
}

/**
 * @brief Erase all values
 * @tparam Key Key type
 * @tparam Value Value type
 * @param key Key
 */
template<class Key, class Value>
void TwoMap<Key, Value>::clear(const Key& key) {
    first_.erase(key);
    second_.erase(key);
}

/**
 * @brief Swap values and
 * erase the second value
 * @tparam Key Key type
 * @tparam Value Value type
 * @param key Key
 */
template<class Key, class Value>
void TwoMap<Key, Value>::swap(const Key& key) {
    Ws::log.debug
        << "(TwoMap::swap) Enter";
    Ws::log.debug
        << "(TwoMap::swap) first_[key] = std::move(second_[key]);";
    first_[key] = std::move(second_[key]);
    Ws::log.debug
        << "(TwoMap::swap) first_[key] = std::move(second_[key]); -- done";
    Ws::log.debug
        << "(TwoMap::swap) second_.erase(key);";
    second_.erase(key);
    Ws::log.debug
        << "(TwoMap::swap) second_.erase(key); -- done";
    Ws::log.debug
        << "(TwoMap::swap) Exit";
}

} // namespace Ws