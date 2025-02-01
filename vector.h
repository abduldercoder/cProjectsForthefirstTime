#ifndef VECTOR_H
#define VECTOR_H
#include<iostream>
#include<stdexcept>
#include<memory>
#include<initializer_list>
#include<algorithm>



template<typename T>
class Vector{
public:
    class Iterator;
    class ConstIterator;

    using value_type = T;
    using size_t = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = std::size_t;
		
    using difference_type = std::ptrdiff_t;
    using iterator = Vector::Iterator;
    using const_iterator = Vector::ConstIterator;
private:
    size_t sz;
    size_t max_sz;
    pointer values;
    static const size_t min_sz = 10;

    void memory_alloc(size_t new_sz){
        pointer new_values = new value_type [new_sz];
        for(size_t i = 0; i < sz; ++i){
            new_values[i] = values[i];
        }
        delete[]values;
        values = new_values;
        max_sz = new_sz;
    }
    size_t upgrade(){
        return max_sz*2;
    }

public:
    Vector() : sz(0) , max_sz(min_sz), values(new value_type[max_sz]){}
    Vector(size_t n) : sz(0), max_sz(n < min_sz ? min_sz : n), values(new value_type[max_sz]){}
    Vector(std::initializer_list<value_type> list) : sz(list.size()), max_sz(sz < min_sz ? min_sz : sz), values(new value_type [max_sz]){
        size_t counter = 0;
        for(auto& i : list){
            values[counter++] = i;
        }
    }
    Vector (const Vector& original) : sz(original.sz), max_sz(original.max_sz), values(new value_type[max_sz]){
        for(size_t i = 0 ; i<sz ; ++i){
            values[i] = original.values[i];
        }
    }
    Vector(std::initializer_list list): sz(list.size()), 

    ~Vector(){
        delete[]values;
        values = nullptr;
    }
    Vector& operator=(Vector rhs){
        std::swap(sz, rhs.sz);
        std::swap(max_sz, rhs.max_sz);
        std::swap(values, rhs.values);
        return *this;
    }
    size_t size() const{
        return sz;
    }
    bool empty() const{
        if(sz != 0)
            return false;
        else
            return true;
    }
    void clear(){
            sz = 0;
    }
    void reserve(size_t n){
        if(max_sz<n) {
            memory_alloc(n);
            max_sz = n;
        }
    }
    void shrink_to_fit(){
        if(max_sz > sz) {
            max_sz = sz;
            memory_alloc(sz);
        }
    }
    void push_back(const_reference x){
        if(sz >= max_sz) {
            memory_alloc(upgrade());
        }
            values[sz++] = x;
    }
    void pop_back(){
        if(sz <= 0)
            throw std::runtime_error("Already empty");
        --sz;
    }
    reference operator[](size_t index){
        if(index >= sz)
            throw std::runtime_error("Index out of range");
        return values[index];
    }
    const_reference operator[](size_t index) const{
        if(index >= sz)
            throw std::runtime_error("Index out of range");
        return values[index];
    }
    size_t capacity() const{
        return max_sz;
    }
    friend std::ostream& operator<<(std::ostream& out, const Vector& vec){
        out<<"[";
        for(int i = 0; i < vec.sz; ++i){
            out<<vec.values[i];
            if(i < vec.sz-1)
                out<<", ";
        }
        out<<"]";
        return out;
    }

    iterator begin(){
        return iterator(values);
    }
    iterator end(){
        return iterator(values + sz);
    }
  	
  
  	
    const_iterator begin() const{
        return const_iterator(values);
    }
    const_iterator end() const{
        return const_iterator(values + sz);
    }

    iterator insert(const const_iterator& pos, const_reference val) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) > sz)
            throw std::runtime_error("Iterator out of bounds");

        size_type current = static_cast<size_type>(diff);
        if (sz >= max_sz)
            reserve(max_sz * 2);
        for (auto i = sz; i-- > current;)
            values[i + 1] = values[i];
        values[current] = val;
        ++sz;
        return iterator(values + current);
    }

    iterator erase(const const_iterator& pos) {
        auto diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) >= sz)
            throw std::runtime_error("Iterator out of bounds");

        size_type current = static_cast<size_type>(diff);
        for (auto i = current; i < sz - 1; ++i)
            values[i] = values[i + 1];
        --sz;
        return iterator(values + current);
    }

    friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
        return lop.operator->() - rop.operator->();
    }

    class Iterator{
    public:
        using value_type = Vector::value_type;
        using reference = Vector::reference;
        using pointer = Vector::pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;
    private:
        pointer ptr;
    public:
        Iterator() : ptr(nullptr){}
        Iterator(pointer ptr) : ptr(ptr){};
        reference operator*() const{
            return *ptr;
        }
        pointer operator->() const{
            return ptr;
        }
        bool operator==(const const_iterator& it) const{
            if(ptr == it.operator->())
                return true;
            else
                return false;

        }
        bool operator!=(const const_iterator& it) const{
            if(ptr != it.operator->())
                return true;
            else
                return false;

        }
        iterator& operator++(){
            ++ptr;
            return *this;
        }
        iterator& operator++(int){
            iterator temp;
            temp = *this;
            ++ptr;
            return temp;
        }
        operator const_iterator() const{
            return const_iterator(ptr);
        }

    };
  
  

    class ConstIterator {
    public:
        using value_type = Vector::value_type;
        using reference = Vector::const_reference;
        using pointer = Vector::const_pointer;
        using difference_type = Vector::difference_type;
        using iterator_category = std::forward_iterator_tag;
    private:
        pointer ptr;
    public:
        ConstIterator() : ptr(nullptr){}
        ConstIterator(pointer ptr) : ptr(ptr){};
        reference operator*() const{
            return *ptr;
        }
        pointer operator->() const{
            return ptr;
        }
        bool operator==(const const_iterator& it) const{
            if(ptr == it.ptr)
                return true;
            else
                return false;

        }
        bool operator!=(const const_iterator& it) const{
            if(ptr != it.ptr)
                return true;
            else
                return false;

        }
        const_iterator& operator++(){
            ++ptr;
            return *this;
        }
        const_iterator& operator++(int){
            const_iterator temp;
            temp = *this;
            ++ptr;
            return temp;
        }

    };
};

#endif 