////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <update/preamble.hpp>
#include <mithril/class/ClassData.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/field/Field.hpp>

namespace yq::mithril::update {
    struct UClass : public U<Class> {
        static UClass&      get(Class);
        
        using Resolve       = std::variant<std::monostate,Class,Field>;
        using ResolveMap    = std::map<std::string,Resolve,IgCase>;

        const Document      doc;

        //Class::SharedData   def;
        ResolveMap          resolve;
        
        UClass(Class);
        void                flash(FF);

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
}
