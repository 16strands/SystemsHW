/*
 * Implementation of Evictor class using least-recently-used policy (LRU).
 */

#pragma once

#include "evictor.hh"

#include <list>
#include <unordered_map>

// FIFO queue evictor that always remove the oldest key.
class LRUEvictor : public Evictor {
 public:
  LRUEvictor() = default;
  ~LRUEvictor() = default;

  // Inform evictor that a certain key has been set or get:
  void touch_key(const key_type& key) ;

  // Request evictor for the next key to evict, and remove it from evictor.
  // If evictor doesn't know what to evict, return an empty key ("").
  key_type evict() ;

 private:
  using recency_type = std::list<key_type>;

  // List of all keys in Evictor, ordered by recency,
  // so that the most recently used key is in the front:
  recency_type keys_;

  // Mapping from key to its location (iterator) in the list:
  std::unordered_map<key_type, recency_type::const_iterator> key_index_;
};
