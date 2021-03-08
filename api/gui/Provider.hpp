////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

template <typename T> class Vector;

class Root;

template <typename T>
struct Provider {
    Provider() {}
    virtual ~Provider(){}
    virtual Vector<T>       all() const = 0;
    virtual bool            can_create() const { return false; }
    virtual T               create(const QString&, const Root*) const { return T{}; }
};
