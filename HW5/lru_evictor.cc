/*
 * Implementation of Evictor class using least-recently-used policy (LRU).
 */

#include "lru_evictor.hh"

// Add key to front of queue if necessary, otherwise move it there:
void
LRUEvictor::touch_key(const key_type& key)
{
  const auto where = key_index_.find(key);
  if (where != key_index_.end()) {
    keys_.erase(where->second);
  }

  keys_.push_front(key);
  key_index_[key] = keys_.begin();
}


// Remove and return last key in list.
key_type
LRUEvictor::evict()
{
  if (keys_.empty()) {
    return "";
  }

  const auto key = keys_.back();
  keys_.pop_back();
  key_index_.erase(key);

  return key;
}
