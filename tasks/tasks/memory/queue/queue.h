#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

class Queue {
public:
    Queue() {
        data_ = nullptr;
        capacity_ = 0;
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    };
    Queue(const Queue& rhs) : Queue() {
        for (size_t i = 0; i < rhs.size_; ++i) {
            Push(rhs.data_[(rhs.head_ + i) % rhs.capacity_]);
        }
    };

    Queue(Queue&& rhs) noexcept {
        data_ = rhs.data_;
        capacity_ = rhs.capacity_;
        size_ = rhs.size_;
        head_ = rhs.head_;
        tail_ = rhs.tail_;

        rhs.data_ = nullptr;
        rhs.capacity_ = 0;
        rhs.size_ = 0;
        rhs.head_ = 0;
        rhs.tail_ = 0;
    };
    Queue& operator=(Queue rhs) {
        Swap(rhs);
        return *this;
    };

    ~Queue() {
        delete[] data_;
    };

    void Swap(Queue& rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(tail_, rhs.tail_);
        std::swap(head_, rhs.head_);
    };

    void Push(int value) {
        if (size_ == capacity_) {
            Reallocate();
        }
        data_[tail_] = value;
        tail_ = (tail_ + 1) % capacity_;
        ++size_;
    };
    void Pop() {
        if (size_ == 0) {
            throw std::out_of_range("cannot pop from empty queue");
        }

        head_ = (head_ + 1) % capacity_;
        --size_;
    };

    int& Front() {
        if (size_ == 0) {
            throw std::out_of_range("cannot front from empty queue");
        }
        return data_[head_];
    };
    const int& Front() const {
        if (size_ == 0) {
            throw std::out_of_range("cannot front from empty queue");
        }

        return data_[head_];
    };

    int& Back() {
        if (size_ == 0) {
            throw std::out_of_range("cannot back from empty queue");
        }
        return data_[(tail_ - 1 + capacity_) % capacity_];
    };
    const int& Back() const {

        if (size_ == 0) {
            throw std::out_of_range("cannot back from empty queue");
        }

        return data_[(tail_ - 1 + capacity_) % capacity_];
    };

    size_t Size() const {
        return size_;
    };
    bool Empty() const {
        return size_ == 0;
    };
    void Clear() {
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    };

private:
    static constexpr size_t INITIAL_CAPACITY = 8;
    static constexpr size_t GROWTH_FACTOR = 2;

    void Reallocate() {
        size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * GROWTH_FACTOR;

        int* new_data = new int[new_capacity];

        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[(head_ + i) % capacity_];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
        head_ = 0;
        tail_ = size_;
    };

    int* data_ = nullptr;
    size_t capacity_ = 0;
    size_t size_ = 0;
    size_t head_ = 0;  // индекс первого элемента
    size_t tail_ = 0;  // индекс позиции для следующего Push
};
