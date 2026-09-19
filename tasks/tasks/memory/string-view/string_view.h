#pragma once

#include <stdexcept>
#include <string>

class StringView {

public:
  StringView(const std::string &str, size_t pos = 0,
             size_t len_pos = std::string::npos) {
    if (pos >= str.size()) {
      throw std::out_of_range("substring pos can not be out of actual string");
    } else {
      if (len_pos == std::string::npos || len_pos > str.size() - pos) {
        size_ = str.size() - pos;

      } else {
        size_ = len_pos;
      }
    }
    start_ = &str[pos];
  }

  StringView(const char *ptr) {
    if (size_ != 0) {
      size_ = std::strlen(ptr);
      start_ = ptr;
    }
  }

  StringView(const char *ptr, size_t len_pos) {
    if (size_ != 0 && len_pos != 0) {
      start_ = ptr;
      size_ = len_pos;
    }
  }

  char operator[](size_t i) const { return start_[i]; }

  size_t Size() const { return size_; }

private:
  const char *start_ = nullptr;
  size_t size_ = 0;
};
