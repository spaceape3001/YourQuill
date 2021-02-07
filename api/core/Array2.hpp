#pragma once

#include <vector>
#include <stddef.h>
#include "IntRange.hpp"

/*
    Simple 2-D XArray 
*/


template <typename T>
class Array2 {
public:


    Array2() : m_rows(0), m_cols(0) {}
    Array2(const Array2&);
    Array2(Array2&&);
    
    Array2& operator=(const Array2&);
    Array2& operator=(Array2&&);
    
    typename std::vector<T>::const_iterator    begin() const;
    typename std::vector<T>::const_iterator    end() const;
    typename std::vector<T>::const_iterator    cbegin() const;
    typename std::vector<T>::const_iterator    cend() const;
    typename std::vector<T>::iterator          begin();
    typename std::vector<T>::iterator          end();
    
    
    ~Array2() {}
    
    bool        empty() const { return m_data.empty(); }
    
    IntRange<int>    rows() const { return range(0, m_rows); }
    IntRange<int>    columns() const { return range(0, m_cols); }
    
    void        resize(int rows, int cols);
    void        fill(const T&);
    int         size() const { return m_data.size(); }
    
    void        clear() { m_data.clear(); }
    
    T&          operator()(int,int);
    const T&    operator()(int,int) const;
    
    //int         row_count() const { return m_rows; }
    //int         column_count() const { return m_cols; }
    
    int         num_rows() const { return m_rows; }
    int         num_columns() const { return m_cols; }
    
    bool        isValid(int, int) const;
    
    void        append_rows(int cnt) ;
    
    const     T*  data() const { return m_data.data(); }
    T*          data()  { return m_data.data(); }
    size_t          bytes() const { return m_data.size() * sizeof(T); }

    
protected:

    int         index(int,int) const;

    std::vector<T>   m_data;
    int         m_rows, m_cols;
};


template <typename T>
Array2<T>::Array2(const Array2& cp)
{
    operator=(cp);
}


template <typename T>
Array2<T>::Array2(Array2&& mv)
{
    operator=(std::move(mv));
}

template <typename T>
Array2<T>& Array2<T>::operator=(const Array2&cp)
{
    m_data      = cp.m_data;
    m_rows      = cp.m_rows;
    m_cols      = cp.m_cols;
    return *this;
}

template <typename T>
Array2<T>& Array2<T>::operator=(Array2&&mv)
{
    m_data      = std::move(mv.m_data);
    m_rows      = mv.m_rows;
    m_cols      = mv.m_cols;
    mv.m_rows = mv.m_cols = 0;
    return *this;
}


template <typename T>
typename std::vector<T>::const_iterator    Array2<T>::begin() const
{
    return m_data.begin();
}


template <typename T>
typename std::vector<T>::const_iterator    Array2<T>::end() const
{
    return m_data.end();
}

template <typename T>
typename std::vector<T>::const_iterator    Array2<T>::cbegin() const
{
    return m_data.begin();
}

template <typename T>
typename std::vector<T>::const_iterator    Array2<T>::cend() const
{
    return m_data.end();
}


template <typename T>
typename std::vector<T>::iterator          Array2<T>::begin()
{
    return m_data.begin();
}

template <typename T>
typename std::vector<T>::iterator          Array2<T>::end()
{
    return m_data.end();
}

template <typename T>
void        Array2<T>::resize(int rows, int cols)
{
    m_rows  = rows;
    m_cols  = cols;
    m_data.resize(rows * cols);
}

template <typename T>
void        Array2<T>::fill(const T& v)
{
    m_data.fill(v);
}



template <typename T>
T&          Array2<T>::operator()(int i,int j)
{
    return m_data[index(i,j)];
}

template <typename T>
const T&    Array2<T>::operator()(int i,int j) const
{
    return m_data[index(i,j)];
}


template <typename T>
bool        Array2<T>::isValid(int r, int c) const
{
    return (r>=0) && (r<m_rows) && (c>=0) && (c<m_cols);
}

template <typename T>
void        Array2<T>::append_rows(int cnt) 
{
    m_rows += cnt;
    m_data.resize(m_rows * m_cols);
}


template <typename T>
int         Array2<T>::index(int r,int c) const
{
    return r * m_cols + c;
}

/*! \brief Makes an array with data wrapping on new columns
*/
template <typename T>
Array2<T>   col_array_cols(const std::vector<T>& data, int cols)
{
    Array2<T>   ret;
    if(cols < 1)
        cols    = 1;
    int         rows    = (data.size() + cols - 1) / cols;
    if(rows < 1)
        rows    = 1;
    ret.resize(rows, cols);
    for(int i=0;i<cols;++i)
        for(int j=0;j<rows;++j)
    {
        size_t  n   = (size_t) i * (size_t) rows + (size_t) j;
        if(n >= data.size())
            continue;
        ret(j,i)   = data[n];
    }
    return ret;
}

