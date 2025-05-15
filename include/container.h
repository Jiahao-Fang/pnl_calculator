#pragma once

#include "type_def.h"
#include <vector>
#include <iostream>
#include <cassert>
namespace pnl {

// Ring buffer implementation using vector
class RingBuffer {
public:
    explicit RingBuffer(size_t initial_capacity = 256)
        : data_(initial_capacity), capacity_(initial_capacity) {
        // Ensure capacity is a power of 2
        assert((capacity_ & (capacity_ - 1)) == 0 && "Capacity must be a power of 2");
    }

    void push(const Position& pos) {
        if (size_ == capacity_) {
            resize(capacity_ << 1); // Multiply by 2 using bit shift
        }
        data_[tail_] = pos;
        tail_ = (tail_ + 1) & (capacity_ - 1); // Fast modulo for power of 2
        ++size_;
        
    }

    Position& front() {
        if (empty()) throw std::runtime_error("front from empty buffer");
        return data_[head_];
    }

    void pop() {
        if (empty()) throw std::runtime_error("pop from empty buffer");
        head_ = (head_ + 1) & (capacity_ - 1); // Fast modulo for power of 2
        --size_;
    }

    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

private:
    void resize(size_t new_capacity) {
        std::vector<Position> new_data(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[(head_ + i) % capacity_];
        }
        data_ = std::move(new_data);
        head_ = 0;
        tail_ = size_;
        capacity_ = new_capacity;
    }

    std::vector<Position> data_;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t size_ = 0;
    size_t capacity_;
};

// Stack implementation using vector
class Stack {
public:
    void push(const Position& pos) {
        data_.push_back(pos);
    }

    Position& front() {
        return data_.back();
    }

    void pop() {
        data_.pop_back();
    }

    bool empty() const {
        return data_.empty();
    }

private:
    std::vector<Position> data_;
};

} // namespace pnl 