////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/Py.hpp>

namespace yq::doodler {
    class Py0Info : public PyInfo {
    public:
        template <typename T> class Writer;
        Py0Info(std::string_view zName, PyInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! A physical object
    
        This represents a physical object, the number represents the number of coordinates needed to describe it.
    */
    class Py0 : public Py {
        YQ_OBJECT_INFO(Py0Info)
        YQ_OBJECT_DECLARE(Py0, Py)
    public:
    
        static void init_info();

    protected:
        Py0(Project&);
        Py0(Project&, const Py0&);
        ~Py0();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&);
        
        
    private:
        Py0(const Py0&) = delete;
        Py0(Py0&&) = delete;
        Py0& operator=(const Py0&) = delete;
        Py0& operator=(Py0&&) = delete;
    };

}
