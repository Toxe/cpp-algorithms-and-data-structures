#pragma once

#include <iterator>
#include <numeric>
#include <vector>

template <typename T>
class CustomVector {
public:
    class CustomVectorIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        CustomVectorIterator(pointer ptr) : ptr_{ptr} { }

        CustomVectorIterator& operator++()
        {
            ++ptr_;
            return *this;
        }

        CustomVectorIterator operator++(int)
        {
            const CustomVectorIterator tmp = *this;
            ++ptr_;
            return tmp;
        }

        CustomVectorIterator& operator--()
        {
            --ptr_;
            return *this;
        }

        CustomVectorIterator operator--(int)
        {
            const CustomVectorIterator tmp = *this;
            --ptr_;
            return tmp;
        }

        CustomVectorIterator& operator+=(const difference_type n)
        {
            ptr_ += n;
            return *this;
        }

        CustomVectorIterator& operator-=(const difference_type n)
        {
            ptr_ -= n;
            return *this;
        }

        reference operator*() const { return *ptr_; }
        reference operator[](const difference_type n) const { return *(ptr_ + n); }

        friend CustomVectorIterator operator+(const difference_type n, const CustomVectorIterator& a) { return CustomVectorIterator{a.ptr_ + n}; }
        friend CustomVectorIterator operator+(const CustomVectorIterator& a, const difference_type n) { return CustomVectorIterator{a.ptr_ + n}; }
        friend CustomVectorIterator operator-(const CustomVectorIterator& a, const difference_type n) { return CustomVectorIterator{a.ptr_ - n}; }

        friend bool operator==(const CustomVectorIterator& a, const CustomVectorIterator& b) { return a.ptr_ == b.ptr_; };
        friend bool operator!=(const CustomVectorIterator& a, const CustomVectorIterator& b) { return a.ptr_ != b.ptr_; };
        friend bool operator<(const CustomVectorIterator& a, const CustomVectorIterator& b) { return a.ptr_ < b.ptr_; }
        friend bool operator>(const CustomVectorIterator& a, const CustomVectorIterator& b) { return a.ptr_ > b.ptr_; }
        friend bool operator<=(const CustomVectorIterator& a, const CustomVectorIterator& b) { return a.ptr_ <= b.ptr_; }
        friend bool operator>=(const CustomVectorIterator& a, const CustomVectorIterator& b) { return a.ptr_ >= b.ptr_; }

        // distance between elements
        friend auto operator-(const CustomVectorIterator& a, const CustomVectorIterator& b) { return a.ptr_ - b.ptr_; };

    private:
        pointer ptr_;
    };

    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    CustomVector(const size_type count = 0)
        : vector_(count)
    {
        std::iota(vector_.begin(), vector_.end(), 1);
    }

    size_type size() const { return vector_.size(); }

    reference operator[](const size_type pos) { return vector_[pos]; }
    const_reference operator[](const size_type pos) const { return vector_[pos]; }

    CustomVectorIterator begin() { return CustomVectorIterator{vector_.data()}; }
    CustomVectorIterator end() { return CustomVectorIterator{vector_.data() + vector_.size()}; }

private:
    std::vector<T> vector_;
};
