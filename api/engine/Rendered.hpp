////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/meta/ObjectInfo.hpp>
#include <basic/UniqueID.hpp>
#include <engine/PipelineConfig.hpp>

namespace yq {
    class Rendered;
    
    class RenderedInfo : public ObjectInfo {
    public:
        template <typename C> struct Writer;
        
        const PipelineConfig*   pipeline(std::string_view) const;


        RenderedInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
        
    private:
        std::map<std::string, PipelineConfig*>   m_pipelines;
    };


    /*! \brief Shows up on the screen
    
        If you want it to show up on the viewport, it needs to be renderable, and thus derived
        from this thing.
    */
    class Rendered : public Object, public RefCount, public UniqueID {
        YQ_OBJECT_INFO(RenderedInfo);
        YQ_OBJECT_DECLARE(Rendered, Object)
    public:    
        uint64_t        id() const { return UniqueID::id(); }
        uint64_t        revision() const { return m_revision; }
    
    protected:
        Rendered();
        Rendered(const Rendered&);
        ~Rendered();
    
        void            changed();
    
    private:
        uint64_t   m_revision   = 0;
    };

    using RenderedPtr   = Ref<Rendered>;

}
