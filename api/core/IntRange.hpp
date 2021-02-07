#pragma once

template <typename T>
class IntRange {
public:
    class iterator {
    public:
    
        iterator() : value(0){}
        iterator(T v) : value(v) {}
        
        iterator& operator++() 
        {
            ++value;
            return *this;
        } 
        
        bool operator!=(const iterator& b) const 
        { 
            return value != b.value; 
        }
        
        T operator*()  { return value; }

    private:
        T     value;
    };

    typedef iterator const_iterator;

    IntRange() : low(0), high(0) {}
    IntRange(T _l, T _h) : low(_l), high(_h) {}

    iterator begin() const
    {
        return iterator(low);
    }
    
    iterator end() const
    {
        return iterator(high);
    }

private:
    T     low, high;
};


/*! \brief Creates an iterable object from the specified range */
template <typename T>
IntRange<T> range(T a, T b)
{
    return IntRange<T>(a,b);
}

