#pragma once

#include <initializer_list>
#include <algorithm>
#include <cstddef>

class Deque {
public:
    static constexpr size_t BLOCK_SIZE = 128;

    Deque() {
        buffer_capacity_ = INITIAL_BUFFER_CAPACITY;
        buffer_ = new int*[buffer_capacity_];
        for (size_t i = 0; i < buffer_capacity_; ++i) {
            buffer_[i] = nullptr;
        }
        head_ = 0;
        block_count_ = 0;
        first_index_in_first_block_ = 0;
        last_index_in_last_block_ = 0;
        size_ = 0;
    }

    Deque(const Deque& rhs) : Deque() {
        for (size_t i = 0; i < rhs.Size(); ++i) {
            PushBack(rhs[i]);
        }
    }

    Deque(Deque&& rhs) noexcept {
        buffer_ = rhs.buffer_;
        buffer_capacity_ = rhs.buffer_capacity_;
        head_ = rhs.head_;
        block_count_ = rhs.block_count_;
        first_index_in_first_block_ = rhs.first_index_in_first_block_;
        last_index_in_last_block_ = rhs.last_index_in_last_block_;
        size_ = rhs.size_;

        rhs.buffer_ = nullptr;
        rhs.buffer_capacity_ = 0;
        rhs.head_ = 0;
        rhs.block_count_ = 0;
        rhs.first_index_in_first_block_ = 0;
        rhs.last_index_in_last_block_ = 0;
        rhs.size_ = 0;
    }

    explicit Deque(size_t size) : Deque() {
        for (size_t i = 0; i < size; ++i) {
            PushBack(0);
        }
    }

    Deque(std::initializer_list<int> list) : Deque() {
        for (int value : list) {
            PushBack(value);
        }
    }

    Deque& operator=(Deque rhs) {
        Swap(rhs);
        return *this;
    }

    ~Deque() {
        if (buffer_ == nullptr) {
            return;
        }
        for (size_t i = 0; i < block_count_; ++i) {
            size_t block_index = (head_ + i) % buffer_capacity_;
            delete[] buffer_[block_index];
        }
        delete[] buffer_;
    }

    void Swap(Deque& rhs) {
        std::swap(buffer_, rhs.buffer_);
        std::swap(buffer_capacity_, rhs.buffer_capacity_);
        std::swap(head_, rhs.head_);
        std::swap(block_count_, rhs.block_count_);
        std::swap(first_index_in_first_block_, rhs.first_index_in_first_block_);
        std::swap(last_index_in_last_block_, rhs.last_index_in_last_block_);
        std::swap(size_, rhs.size_);
    }

    void PushBack(int value) {
        if (size_ == 0) {
            buffer_[head_] = new int[BLOCK_SIZE];
            buffer_[head_][0] = value;
            ++block_count_;
            ++size_;
            first_index_in_first_block_ = 0;
            last_index_in_last_block_ = 0;
        }
        else if (last_index_in_last_block_ + 1 < BLOCK_SIZE) {
            int* last_block = buffer_[(head_ + block_count_ - 1) % buffer_capacity_];
            last_block[last_index_in_last_block_ + 1] = value;
            ++last_index_in_last_block_;
            ++size_;
        }
        else {
            Reallocate();

            size_t new_index = (head_ + block_count_) % buffer_capacity_;
            buffer_[new_index] = new int[BLOCK_SIZE];
            buffer_[new_index][0] = value;
            last_index_in_last_block_ = 0;
            ++block_count_;
            ++size_;
        }
    }

    void PopBack() {
        size_t start = (block_count_ == 1) ? first_index_in_first_block_ : 0;

        if (last_index_in_last_block_ > start) {
            --last_index_in_last_block_;
            --size_;
        } else {
            size_t last_block_index = (head_ + block_count_ - 1) % buffer_capacity_;
            delete[] buffer_[last_block_index];
            buffer_[last_block_index] = nullptr;
            --block_count_;
            if (block_count_ == 0) {
                head_ = 0;
                first_index_in_first_block_ = 0;
                last_index_in_last_block_ = 0;
            } else {
                last_index_in_last_block_ = BLOCK_SIZE - 1;
            }
            --size_;
        }
    }

    void PushFront(int value) {
        if (size_ == 0) {
            buffer_[head_] = new int[BLOCK_SIZE];
            buffer_[head_][BLOCK_SIZE - 1] = value;
            block_count_ = 1;
            first_index_in_first_block_ = BLOCK_SIZE - 1;
            last_index_in_last_block_ = BLOCK_SIZE - 1;
            size_ = 1;
        }
        else if (first_index_in_first_block_ > 0) {
            buffer_[head_][first_index_in_first_block_ - 1] = value;
            --first_index_in_first_block_;
            ++size_;
        }
        else {
            if (block_count_ == buffer_capacity_) {
                Reallocate();
            }
            head_ = (head_ - 1 + buffer_capacity_) % buffer_capacity_;
            buffer_[head_] = new int[BLOCK_SIZE];
            buffer_[head_][BLOCK_SIZE - 1] = value;
            first_index_in_first_block_ = BLOCK_SIZE - 1;
            ++block_count_;
            ++size_;
        }
    }

    void PopFront() {
        // Сколько элементов в первом блоке?
        size_t elems_in_first;
        if (block_count_ == 1) {
            elems_in_first = last_index_in_last_block_ - first_index_in_first_block_ + 1;
        } else {
            elems_in_first = BLOCK_SIZE - first_index_in_first_block_;
        }

        if (elems_in_first > 1) {
            ++first_index_in_first_block_;
        } else {
            delete[] buffer_[head_];
            buffer_[head_] = nullptr;
            --block_count_;
            if (block_count_ == 0) {
                head_ = 0;
                first_index_in_first_block_ = 0;
                last_index_in_last_block_ = 0;
            } else {
                head_ = (head_ + 1) % buffer_capacity_;
                first_index_in_first_block_ = 0;
            }
        }
        --size_;
    }

    int& operator[](size_t ind) {
        size_t elems_in_first_block = BLOCK_SIZE - first_index_in_first_block_;

        if (ind < elems_in_first_block) {
            return buffer_[head_][first_index_in_first_block_ + ind];
        } else {
            ind -= elems_in_first_block;
        }

        size_t current_block = (head_ + 1) % buffer_capacity_;

        while (ind >= BLOCK_SIZE) {
            ind -= BLOCK_SIZE;
            current_block = (current_block + 1) % buffer_capacity_;
        }
        return buffer_[current_block][ind];
    }

    int operator[](size_t ind) const {
        size_t elems_in_first_block = BLOCK_SIZE - first_index_in_first_block_;

        if (ind < elems_in_first_block) {
            return buffer_[head_][first_index_in_first_block_ + ind];
        } else {
            ind -= elems_in_first_block;
        }

        size_t current_block = (head_ + 1) % buffer_capacity_;

        while (ind >= BLOCK_SIZE) {
            ind -= BLOCK_SIZE;
            current_block = (current_block + 1) % buffer_capacity_;
        }
        return buffer_[current_block][ind];
    }

    size_t Size() const {
        return size_;
    }

    void Clear() {
        if (buffer_ == nullptr) {
            return;
        }
        for (size_t i = 0; i < block_count_; ++i) {
            size_t ind = (head_ + i) % buffer_capacity_;
            if (buffer_[ind] != nullptr) {
                delete[] buffer_[ind];
                buffer_[ind] = nullptr;
            }
        }
        head_ = 0;
        block_count_ = 0;
        first_index_in_first_block_ = 0;
        last_index_in_last_block_ = 0;
        size_ = 0;
    }

private:
    static constexpr size_t INITIAL_BUFFER_CAPACITY = 8;
    static constexpr size_t BUFFER_GROWTH_FACTOR = 2;

    void Reallocate() {
        if (block_count_ < buffer_capacity_) {
            return;
        }

        size_t old_capacity = buffer_capacity_;
        int** old_buffer = buffer_;

        size_t new_capacity = old_capacity * BUFFER_GROWTH_FACTOR;

        int** new_buffer = new int*[new_capacity];

        for (size_t i = 0; i < new_capacity; ++i) {
            new_buffer[i] = nullptr;
        }

        for (size_t i = 0; i < block_count_; ++i) {
            size_t old_index = (head_ + i) % old_capacity;
            new_buffer[i] = old_buffer[old_index];
        }

        delete[] old_buffer;

        buffer_ = new_buffer;
        buffer_capacity_ = new_capacity;
        head_ = 0;
    }

    int** buffer_ = nullptr;                     // кольцевой буфер указателей на блоки
    size_t buffer_capacity_ = 0;                // количество ячеек в буфере
    size_t head_ = 0;                           // индекс первого блока в буфере
    size_t block_count_ = 0;                    // количество используемых блоков
    size_t first_index_in_first_block_ = 0;     // начало данных в первом блоке
    size_t last_index_in_last_block_ = 0;       // конец данных в последнем блоке
    size_t size_ = 0;                           // количество элементов в деке
};