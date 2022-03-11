////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/CacheFwd.hpp>
#include <util/Set.hpp>


template <typename T>
struct ADSet {
    Set<T>      def;
    Set<T>      all;
};

using CPair         = std::pair<Class,Class>;
using FPair         = std::pair<Class,Field>;

template <typename T>
struct AddRem {
    Set<T>      add, rem;
    
    
    AddRem(){}
    AddRem(const Set<T>& oldSet, const Set<T>& newSet) : add(newSet-oldSet), rem(oldSet-newSet) {}
    AddRem(const Set<T>& oldSet, const Vector<T>& newSet) : 
        AddRem(oldSet, makeSet(newSet)) {}
    AddRem(const Vector<T>& oldSet, const Set<T>& newSet) : 
        AddRem(makeSet(oldSet), newSet) {}
    AddRem(const Vector<T>& oldSet, const Vector<T>& newSet) : 
        AddRem(makeSet(oldSet), makeSet(newSet)) {}
    operator bool() const { return !(add.empty() && rem.empty()); }
};
