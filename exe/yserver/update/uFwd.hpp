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

using ClassSet      = Set<Class>;
using FieldSet      = Set<Field>;
using TagSet        = Set<Tag>;
