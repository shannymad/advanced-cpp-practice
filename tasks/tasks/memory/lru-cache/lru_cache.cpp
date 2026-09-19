#include "lru_cache.h"

LruCache::LruCache(size_t max_size) : max_size_(max_size) {}

void LruCache::Set(const std::string &key, const std::string &value) {
  auto it = map_.find(key);
  if (map_.find(key) != map_.end()) {
    it->second->second = value;
    items_.splice(items_.begin(), items_, it->second);
  } else {
    items_.push_front({key, value});
    map_[key] = items_.begin();

    if (items_.size() > max_size_) {
      auto old_key = items_.back().first;
      map_.erase(old_key);
      items_.pop_back();
    }
  }
}

bool LruCache::Get(const std::string &key, std::string *value) {
  auto it = map_.find(key);
  if (map_.find(key) != map_.end()) {
    *value = it->second->second;
    items_.splice(items_.begin(), items_, it->second);
    return true;
  }
  return false;
}
