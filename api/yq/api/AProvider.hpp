////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

template <typename T> class Vector;

template <typename T>
struct AProvider {
    AProvider() {}
    virtual ~AProvider(){}
    virtual Vector<T>       all() const = 0;
    virtual bool            can_create() const { return false; }
    virtual T               create(const QString&) const { return T{}; }
};
