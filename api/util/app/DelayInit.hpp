////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <util/c++/Concat.hpp>

namespace yq {

/*! \brief Simple API to permit a delayed initialization

    There can be a potential race issue with initializing these and 
    then calling "initialize()" because there's several operations for
    which this object could be registered in the manifest while the derived
    initialized class is still under construction.
*/

class  DelayInit {
public:
    struct Ctor;
    static void         init_all(bool fRepeat=true);

protected:

    DelayInit();
    virtual void        initialize(){}

private:

    static DelayInit*   current(DelayInit*);
    static bool         init_pass();
    
    DelayInit(const DelayInit&) = delete;
    DelayInit(DelayInit&&) = delete;
    void    operator=(const DelayInit&) = delete;
    void    operator=(DelayInit&&) = delete;
 
    DelayInit*      m_next;
};

/*! \brief Simple class that runs a function during initialization

    Useful for running things on startup.
*/
struct DelayInit::Ctor : public DelayInit {
    void(*fn)();
    
    Ctor(void(*_fn)()) : fn(_fn) {}
    void    initialize() override { fn(); }
};

/*! \brief Macro to invoke code at program startup */
#define INVOKE(...) \
    namespace { \
        static DelayInit::Ctor   CONCAT(s_invoke_, __COUNTER__) ( []() { __VA_ARGS__ } ); \
    }
}
