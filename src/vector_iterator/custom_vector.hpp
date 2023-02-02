#pragma once

#include <compare>
#include <iterator>
#include <numeric>
#include <vector>

template <typename T>
class CustomVector {
private:
    template <typename pointer, typename reference>
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;

        constexpr Iterator() : ptr_{} { }
        constexpr explicit Iterator(pointer ptr) : ptr_{ptr} { }

        reference operator*() const { return *ptr_; }

        Iterator& operator++()
        {
            ++ptr_;
            return *this;
        }

        Iterator operator++(int)
        {
            const Iterator tmp{*this};
            ++(*this);
            return tmp;
        }

        Iterator& operator--()
        {
            --ptr_;
            return *this;
        }

        Iterator operator--(int)
        {
            const Iterator tmp{*this};
            --(*this);
            return tmp;
        }

        Iterator& operator+=(const difference_type n)
        {
            ptr_ += n;
            return *this;
        }

        Iterator& operator-=(const difference_type n)
        {
            ptr_ -= n;
            return *this;
        }

        Iterator operator+(const difference_type n) const { return Iterator{ptr_ + n}; }
        Iterator operator-(const difference_type n) const { return Iterator{ptr_ - n}; }
        friend Iterator operator+(const difference_type n, const Iterator& a) { return Iterator{a.ptr_ + n}; }
        friend difference_type operator-(const Iterator& a, const Iterator& b) { return a.ptr_ - b.ptr_; };

        reference operator[](const difference_type n) const { return *(ptr_ + n); }

        auto operator<=>(const Iterator& other) const = default;

    private:
        pointer ptr_;
    };

public:
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using iterator = Iterator<pointer, reference>;
    using const_iterator = Iterator<const_pointer, const_reference>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    CustomVector(const size_type count = 0)
        : vector_(count)
    {
        std::iota(vector_.begin(), vector_.end(), 1);
    }

    size_type size() const { return vector_.size(); }

    reference operator[](const size_type pos) { return vector_[pos]; }
    const_reference operator[](const size_type pos) const { return vector_[pos]; }

    iterator begin() { return iterator{vector_.data()}; }
    const_iterator begin() const { return const_iterator{vector_.data()}; }
    const_iterator cbegin() const { return begin(); }

    iterator end() { return iterator{vector_.data() + vector_.size()}; }
    const_iterator end() const { return const_iterator{vector_.data() + vector_.size()}; }
    const_iterator cend() const { return end(); }

    reverse_iterator rbegin() { return reverse_iterator{end()}; }
    const_reverse_iterator rbegin() const { return const_reverse_iterator{end()}; }
    const_reverse_iterator crbegin() const { return rbegin(); }

    reverse_iterator rend() { return reverse_iterator{begin()}; }
    const_reverse_iterator rend() const { return const_reverse_iterator{begin()}; }
    const_reverse_iterator crend() const { return rend(); }

private:
    std::vector<T> vector_;
};
