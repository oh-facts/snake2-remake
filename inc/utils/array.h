#ifndef S2_ARRAY_H
#define S2_ARRAY_H


#include <assert.h>
#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <algorithm>


namespace s2 {
    template <typename T, std::size_t N>
    class array;
    template <typename T, std::size_t N>
    class array_iterator;


    template <typename T, std::size_t N>
    class array_iterator {
    public:
        using reference = typename array<T, N>::reference;
        using pointer = typename array<T, N>::pointer;
        using const_pointer = typename array<T, N>::const_pointer;
        using size_type = typename array<T, N>::size_type;
        using difference_type = typename std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
    public:
        array_iterator(const_pointer _ptr):
            _ptr(_ptr) {}

        array_iterator(const array_iterator& _iter):
            _ptr(_iter._ptr) {}

        array_iterator() noexcept:
            _ptr(nullptr) {}

        ~array_iterator() noexcept {}

        array_iterator& operator= (const array_iterator& _iter) {
            if(*this == _iter) return *this;

            this->_ptr = _iter._ptr;
            return *this;
        }

        constexpr bool operator== (const array_iterator& _iter) const noexcept {
            return (this->_ptr == _iter._ptr);
        }

        constexpr bool operator!= (const array_iterator& _iter) const noexcept {
            return (!(this->_ptr == _iter._ptr));
        }

        array_iterator& operator++ () {
            this->_ptr++;
            return *this;
        }

        array_iterator& operator++ (int _unused) {
            static_cast<void>(_unused);
           array_iterator iter = *this;
            ++(*this);
            return *this;
        }

        array_iterator& operator-- () {
            this->_ptr--;
            return *this;
        }

        array_iterator& operator-- (int _unused) {
            static_cast<void>(_unused);
           array_iterator iter = *this;
            --(*this);
            return iter;
        }

       array_iterator operator+ (const size_type _offset) const {
           array_iterator iter = *this;
            iter._ptr += _offset;
            return iter;
        }

       array_iterator operator- (const size_type _offset) const {
           array_iterator iter = *this;
            iter._ptr -= _offset;
            return iter;
        }

        array_iterator& operator+= (const size_type _offset) {
            this->_ptr += _offset;
            return *this;
        }

        array_iterator& operator-= (const size_type _offset) {
            this->_ptr -= _offset;
            return *this;
        }

        reference operator* () const {
            return *this->_ptr;
        }

        pointer operator-> () const {
            return this->_ptr;
        }

        reference operator[] (const size_type _offset) const {
            return this->_data[_offset];
        }
    private:
        pointer _ptr;
    };

    template <typename T, std::size_t N>
    class array {
    public:
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const pointer;
        using size_type = std::size_t;
        using iterator = array_iterator<T, N>;
    public:
        array(std::initializer_list<value_type> _il) {
            assert(_il.size() < this->size() &&
                   "Too many elements in initializer list");

            size_type i = 0;
            for(value_type value: _il) {
                if(i == this->size()) break;

                this->_data[i++] = value;
            }

            this->_cap = i;
        }

        array(reference _value) noexcept:
            _data(std::fill(this->begin(), this->end(), _value)),
            _cap(this->size() - 1) {}
        
        array(const array& _array) noexcept:
            _data(std::copy(_array->begin(), _array->end(), this->begin())),
            _cap(_array._cap) {}

        array() noexcept:
            _data{}, _cap{} {}

        ~array() {}

        reference at(const size_type _offset) noexcept {
            assert((_offset > 0) &&(_offset >= this->size()) &&
                    "Offset is out of range");            
            return this->_data[_offset];
        }

        const_reference at(const size_type _offset) const noexcept {
            assert((_offset > 0) &&(_offset >= this->size()) &&
                    "Offset is out of range");            
            return this->_data[_offset];
        }

        reference front() noexcept {
            return *this->begin();
        }

        const_reference front() const noexcept {
            return *this->begin();
        }

        reference back() noexcept {
            return *std::prev(this->end());
        }

        const_reference back() const noexcept {
            return *std::prev(this->end());
        }

        bool constexpr empty() const noexcept {
            return (this->begin() == this->end());
        }

        size_type size() const noexcept {
            return N;
        }

        const_pointer data() const noexcept {
            return this->_data;
        }

        pointer data() noexcept {
            return this->_data;
        }

        size_type cap() const noexcept {
            return this->_cap;
        }

        iterator begin() noexcept {
            return iterator(this->_data);
        }

        iterator rbegin() noexcept {
            return iterator(this->end());
        }

        iterator end() noexcept {
            return iterator(&this->_data[this->size()]);
        }

        iterator rend() noexcept {
            return iterator(this->begin());
        }

        iterator cap_end() noexcept {
            return iterator(&this->_data[this->cap() + 1]);
        }

        reference operator[] (size_type _offset) noexcept {
            return this->_data[_offset];
        }

        const_reference operator[] (size_type _offset) const noexcept {
            return this->_data[_offset];
        }
    private:
        value_type _data[N];
        size_type _cap;
    };
}

#endif // S2_ARRAY_H
