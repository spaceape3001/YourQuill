////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Editor.hpp"

YQ_OBJECT_IMPLEMENT(yq::Editor)

namespace yq {
    EditorInfo::EditorInfo(std::string_view n, ObjectInfo& base, const std::source_location& sl) : ObjectInfo(n, base, sl)
    {
    }

    Editor::Editor()
    {
    }
    
    Editor::~Editor()
    {
    }
}
