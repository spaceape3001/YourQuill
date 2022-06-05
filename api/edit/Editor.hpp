////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/meta/ObjectInfo.hpp>
#include <variant>
#include <filesystem>

namespace yq {
    class EditorInfo : public ObjectInfo {
    public:
        
        template <typename C>  struct Writer;
        
        EditorInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
    };
    
    using EditorSpec    = std::variant<std::monostate, uint64_t, std::filesystem::path>;
    
    class Editor : public Object {
        YQ_OBJECT_INFO(EditorInfo)
        YQ_OBJECT_DECLARE(Editor, Object)
    public:
    
        virtual bool    load(EditorSpec) = 0;
        virtual void    draw_imgui() {}
        
        const std::filesystem::path&    file() const { return m_file; }
    protected:

        std::filesystem::path   m_file;
    
        Editor();
        ~Editor();
    };
}
