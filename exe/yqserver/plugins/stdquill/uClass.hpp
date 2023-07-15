////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

struct UClass {
    static UClass&      get(Class c);
    Class::SharedData   data;

#if 0    
    StringCountMap  alias;
    ClassCountMap   base;
    ClassCountMap   derive;
    StringCountMap  prefix;
    ClassCountMap   reverse;
    ClassCountMap   source;
    StringCountMap  suffix;
    ClassCountMap   target;
#endif

};
