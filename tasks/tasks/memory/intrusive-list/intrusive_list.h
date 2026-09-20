#pragma once

#include <algorithm>
#include <iterator>

class ListHook {
public:
  ListHook() : next_(this), prev_(this) {}

  bool IsLinked() const { return next_ != this; }

  void Unlink() {
    prev_->next_ = next_;
    next_->prev_ = prev_;
    next_ = prev_ = this;
  }

  ~ListHook() { Unlink(); }

  ListHook(const ListHook &) = delete;

  ListHook *Next() const { return next_; }

private:
  template <class T> friend class List;

  ListHook *next_;
  ListHook *prev_;

  void LinkBefore(ListHook *other) {
    next_ = other;
    prev_ = other->prev_;
    other->prev_->next_ = this;
    other->prev_ = this;
  }
};

template <typename T> class List {
public:
  class Iterator {
    using IteratorTag = std::bidirectional_iterator_tag;

  public:
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;
    typedef IteratorTag iterator_category;

    Iterator &operator++() {
      current_ = current_->Next();
      return *this;
    }

    Iterator operator++(int) {
      Iterator copy = *this;
      ++(*this);
      return copy;
    }

    Iterator(ListHook *current) : current_(current) {}

    T &operator*() const { return *static_cast<T *>(current_); }

    T *operator->() const { return static_cast<T *>(current_); }

    bool operator==(const Iterator &rhs) const {
      return current_ == rhs.current_;
    }

    bool operator!=(const Iterator &rhs) const {
      return current_ != rhs.current_;
    }

  private:
    ListHook *current_;
  };

  List() {}

  List(const List &) = delete;

  List(List &&other) {
    if (!other.IsEmpty()) {
      other.dummy_.next_->prev_ = &dummy_;
      other.dummy_.prev_->next_ = &dummy_;
      dummy_.next_ = other.dummy_.next_;
      dummy_.prev_ = other.dummy_.prev_;
      other.dummy_.next_ = &other.dummy_;
      other.dummy_.prev_ = &other.dummy_;
    }
  }

  ~List() {
    ListHook *current = dummy_.next_;
    while (current != &dummy_) {
      ListHook *current_next = current->Next();
      current->Unlink();
      current = current_next;
    }
  }

  List &operator=(const List &) = delete;

  List &operator=(List &&other) {
    if (this == &other) {
      return *this;
    }

    ListHook *current = dummy_.next_;
    while (current != &dummy_) {
      ListHook *current_next = current->Next();
      current->Unlink();
      current = current_next;
    }

    if (other.dummy_.next_ != &other.dummy_) {
      other.dummy_.next_->prev_ = &dummy_;
      other.dummy_.prev_->next_ = &dummy_;
      dummy_.next_ = other.dummy_.next_;
      dummy_.prev_ = other.dummy_.prev_;
    }

    other.dummy_.next_ = &other.dummy_;
    other.dummy_.prev_ = &other.dummy_;

    return *this;
  }

  bool IsEmpty() const { return dummy_.next_ == &dummy_; }

  size_t Size() const {
    size_t count = 0;
    ListHook *current = dummy_.next_;
    while (current != &dummy_) {
      ++count;
      current = current->Next();
    }
    return count;
  }

  void PushBack(T *elem) { elem->LinkBefore(&dummy_); }

  void PushFront(T *elem) { elem->LinkBefore(dummy_.next_); }

  T &Front() { return *static_cast<T *>(dummy_.next_); }

  const T &Front() const { return *static_cast<const T *>(dummy_.next_); }

  T &Back() { return *static_cast<T *>(dummy_.prev_); }

  const T &Back() const { return *static_cast<const T *>(dummy_.prev_); }

  void PopBack() { dummy_.prev_->Unlink(); }

  void PopFront() { dummy_.next_->Unlink(); }

  Iterator Begin() { return Iterator(dummy_.next_); }

  Iterator End() { return Iterator(&dummy_); }

  Iterator IteratorTo(T *element) {
    return Iterator(static_cast<ListHook *>(element));
  }

private:
  ListHook dummy_;
};

template <typename T> typename List<T>::Iterator begin(List<T> &list) {
  return list.Begin();
}

template <typename T> typename List<T>::Iterator end(List<T> &list) {
  return list.End();
}
