#pragma once

#include <compare>
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

        constexpr CustomVectorIterator() : ptr_{} { }
        constexpr explicit CustomVectorIterator(pointer ptr) : ptr_{ptr} { }

        reference operator*() const { return *ptr_; }

        CustomVectorIterator& operator++()
        {
            ++ptr_;
            return *this;
        }

        CustomVectorIterator operator++(int)
        {
            const CustomVectorIterator tmp{*this};
            ++(*this);
            return tmp;
        }

        CustomVectorIterator& operator--()
        {
            --ptr_;
            return *this;
        }

        CustomVectorIterator operator--(int)
        {
            const CustomVectorIterator tmp{*this};
            --(*this);
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

        CustomVectorIterator operator+(const difference_type n) const { return CustomVectorIterator{ptr_ + n}; }
        CustomVectorIterator operator-(const difference_type n) const { return CustomVectorIterator{ptr_ - n}; }
        friend CustomVectorIterator operator+(const difference_type n, const CustomVectorIterator& a) { return CustomVectorIterator{a.ptr_ + n}; }

        auto operator<=>(const CustomVectorIterator& other) const = default;

        reference operator[](const difference_type n) const { return *(ptr_ + n); }

        // distance between elements
        friend auto operator-(const CustomVectorIterator& a, const CustomVectorIterator& b) { return a.ptr_ - b.ptr_; };

    private:
        pointer ptr_;
    };

    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using iterator = CustomVectorIterator;
    using reverse_iterator = std::reverse_iterator<CustomVectorIterator>;

    CustomVector(const size_type count = 0)
        : vector_(count)
    {
        std::iota(vector_.begin(), vector_.end(), 1);
    }

    size_type size() const { return vector_.size(); }

    reference operator[](const size_type pos) { return vector_[pos]; }
    const_reference operator[](const size_type pos) const { return vector_[pos]; }

    iterator begin() { return iterator{vector_.data()}; }
    iterator end() { return iterator{vector_.data() + vector_.size()}; }

    reverse_iterator rbegin() { return reverse_iterator{end()}; }
    reverse_iterator rend() { return reverse_iterator{begin()}; }

private:
    std::vector<T> vector_;
};
