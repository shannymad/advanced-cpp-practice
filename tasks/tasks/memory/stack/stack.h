#pragma once

#include <cstddef>
#include <algorithm>
#include <stdexcept>

class Stack {
public:
    Stack() {
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    };
    Stack(const Stack& rhs) : Stack(){
        for (size_t i = 0; i < rhs.size_; ++i) {
            Push(rhs.data_[i]);
        }
    };
    Stack(Stack&& rhs) noexcept {

        data_ = rhs.data_;
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;

        rhs.data_ = nullptr;
        rhs.size_ = 0;
        rhs.capacity_ = 0;

    };
    Stack& operator=(Stack rhs) {
        Swap(rhs);
        return *this;
    };
    ~Stack() {
        delete[] data_;
    };

    void Swap(Stack& rhs) {
        std::swap(data_, rhs.data_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
    };

    void Push(int value) {
        if (size_ == capacity_) {
            
            Reallocate();
        } 

         data_[size_] = value;
        ++size_;

    };
    void Pop() {
        if (size_ == 0) {
            throw std::out_of_range("cannot pop from empty stack");
        };
        --size_;
    };
    int& Top() {
        if (size_ == 0) {
            throw std::out_of_range("cannot top from empty stack");
        };
        return data_[size_ - 1];
    };
    const int& Top() const {
        if (size_ == 0) {
            throw std::out_of_range("cannot top from empty stack");
        };
        return data_[size_ - 1];
    }
    size_t Size() const {
        return size_;
    };
    bool Empty() const {
        return size_ == 0;

    };
    void Clear() {
        size_ = 0;
    };

private:

    void Reallocate() {
        size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        int* new_data = new int[new_capacity];

        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    int* data_ = nullptr;
    size_t size_ = 0; 
    size_t capacity_ = 0;

};