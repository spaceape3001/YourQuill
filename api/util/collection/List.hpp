////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#define YQUILL__API__CORE__TYPE__LIST__H__

#include <list>
//#include <QList>

namespace yq {

    /*! \brief List Container
    */
    template <typename T>
    class List : public std::list<T> {
    public:
        typedef std::list<T>    base_list;
        
                    List(){}
                    
                    template <class InputIterator>
                    List(InputIterator, InputIterator);
                    
                    List(const base_list& toCopy);
                    List(base_list&& toMove);
                    ~List(){}
                    
        List&       operator=(const base_list&);
        List&       operator=(base_list&&);

        T           first(const T& def={}) const;
        T           last(const T& def={}) const;
        
        List&       operator<<(const T& value);
        List&       operator<<(T&&);

        List&       operator+=(const T& value);
        List&       operator+=(T&&);
        List&       operator+=(const std::list<T>&);
    };

    template <typename T>
    List<const T*>  makeConst(const List<T*>&);


    template <typename T>
    List<T>         makeList(const std::list<T>&);

    //template <typename K, typename V, typename C>
    //List<std::pair<K,V>>    makeList(const std::map<K,V,C>&);

    //template <typename T, typename C>
    //List<T>         makeList(const std::set<T,C>&);

    //template <typename K, typename V>
    //List<std::pair<K,V>>    makeList(const std::unordered_map<K,V>&);

    //template <typename T>
    //List<T>         makeList(const std::vector<T>&);

    //template <typename T>
    //QList<T>            qt(const List<T>&);

    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  //
    //  ====================================================================  //
    //                      IMPLEMENTATION!!!!!                               //
    //  ====================================================================  //
    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  //

    template <typename T>
    List<T>::List(const base_list& cp) : 
        base_list(cp) 
    {
    }
        
    template <typename T>
    List<T>::List(base_list&& mv) : 
        base_list(std::move(mv))
    {
    }

    template <typename T>
        template <class InputIterator>
    List<T>::List(InputIterator a, InputIterator b) :
        base_list(a, b)
    {
    }

    template <typename T>
    List<T>&      List<T>::operator=(const base_list&cp)
    {
        base_list::operator=(cp);
        return *this;
    }

    template <typename T>
    List<T>&      List<T>::operator=(base_list&&mv)
    {
        base_list::operator=(std::move(mv));
        return *this;
    }

        template <typename T>
    T           List<T>::first(const T& def) const
    {
        if(base_list::empty())
            return def;
        else
            return base_list::front();
    }

    template <typename T>
    T           List<T>::last(const T& def) const
    {
        if(base_list::empty())
            return def;
        else
            return base_list::back();
    }


    template <typename T>
    List<T>&      List<T>::operator<<(const T& value)
    {
        base_list::push_back(value);
        return *this;
    }

    template <typename T>
    List<T>&      List<T>::operator<<(T&& value)
    {
        base_list::push_back(std::move(value));
        return *this;
    }

    template <typename T>
    List<T>&      List<T>::operator+=(const T& value)
    {
        base_list::push_back(value);
        return *this;
    }

    template <typename T>
    List<T>&      List<T>::operator+=(T&& value)
    {
        base_list::push_back(std::move(value));
        return *this;
    }

    template <typename T>
    List<T>&      List<T>::operator+=(const std::list<T>&src)
    {
        base_list::insert(base_list::end(), src.begin(), src.end());
        return *this;
    }


    template <typename T>
    bool    operator==(const List<T>& lhs, const List<T>& rhs)
    {
        if(lhs.size() != rhs.size())
            return false;
        auto a	= lhs.begin();
        auto b	= rhs.begin();
        for(; a!=lhs.end(); ++a, ++b){
            assert(b!=rhs.end());
            if(*a != *b)
                return false;
        }
        assert(b==rhs.end());
        return true;
    }

    template <typename T>
    bool    operator!=(const List<T>& lhs, const List<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T>
    List<const T*>  makeConst(const List<T*>&src)
    {
        return List<const T*>(src.begin(), src.end());
    }

    template <typename T>
    List<T>         makeList(const std::list<T>&src)
    {
        return List<T>(src.begin(), src.end());
    }


    //template <typename T>
    //QList<T>            qt(const List<T>&src)
    //{
        //return QList<T>(src.begin(), src.end()); 
    //}


    //template <typename T>
    //QList<T>            qtList(const List<T>& src)
    //{
        //return QList<T>(src.begin(), src.end()); 
    //}

    //template <typename T>
    //void    sort(QList<T>& list)
    //{
        //std::stable_sort(list.begin(), list.end());
    //}

    //template <typename T, typename P>
    //void    sort(QList<T>& list, P p)
    //{
        //std::stable_sort(list.begin(), list.end(), p);
    //}
}

#ifdef YQUILL__API__CORE__TYPE__HASH__H__
    #include "HashListImpl.hxx"
#endif
#ifdef YQUILL__API__CORE__TYPE__MAP__H__
    #include "MapListImpl.hxx"
#endif
#ifdef YQUILL__API__CORE__TYPE__SET__H__
    #include "ListSetImpl.hxx"
#endif
#ifdef YQUILL__API__CORE__TYPE__VECTOR__H__
    #include "ListVectorImpl.hxx"
#endif
