#pragma once

#include "type_def.h"
#include <vector>

namespace pnl {

// Ring buffer implementation using vector
class RingBuffer {
public:
    void push(const Position& pos) {
        if (size_ == data_.size()) {
            data_.push_back(pos);
        } else {
            data_[tail_] = pos;
        }
        tail_ = (tail_ + 1) % data_.size();
        size_++;
    }

    Position& front() {
        return data_[head_];
    }

    void pop() {
        head_ = (head_ + 1) % data_.size();
        size_--;
    }

    bool empty() const {
        return size_ == 0;
    }

private:
    std::vector<Position> data_;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t size_ = 0;
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