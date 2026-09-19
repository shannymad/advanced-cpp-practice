#pragma once

#include <list>
#include <string>
#include <unordered_map>

class LruCache {
public:
  LruCache(size_t max_size);

  void Set(const std::string &key, const std::string &value);

  bool Get(const std::string &key, std::string *value);

private:
  using ListType = std::list<std::pair<std::string, std::string>>;

  using ListIterator = ListType::iterator;

  size_t max_size_ = 0;

  ListType items_;
  std::unordered_map<std::string, ListIterator> map_;
};
