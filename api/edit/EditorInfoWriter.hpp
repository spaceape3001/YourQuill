////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <edit/Editor.hpp>

namespace yq {
    template <typename C>
    class EditorInfo::Writer : public ObjectInfo::Writer<C> {
    public:
    
        Writer(EditorInfo* editorInfo) : ObjectInfo::Writer<C>(editorInfo)
        {
        }
        
        Writer(EditorInfo& editorInfo) : Writer(&editorInfo)
        {
        }
    };
}
