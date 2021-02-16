#pragma once


template <typename T>
struct ReverseIter {
    ReverseIter(const T& _c) : c(_c) {}
    typename T::const_reverse_iterator begin() const { return c.rbegin(); }
    typename T::const_reverse_iterator end() const { return c.rend(); }
    const T& arg() const { return c; }
private:
    const T& c;
};

template <typename T>
ReverseIter<T> reverse(const T& c) 
{ 
    return ReverseIter<T>(c); 
}

template <typename T>
bool  operator==(const T&a, const ReverseIter<T>& b)
{
    if(a.size() != b.arg().size())
        return false;
    auto itr = a.begin();
    auto jtr = b.begin();
    for(; itr != a.end(); ++itr, ++jtr){
        if(*itr != *jtr)
            return false;
    }
    return true;
}

template <typename T>
bool  operator!=(const T&a, const ReverseIter<T>& b)
{
    return !(a==b);
}
