////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/Py.hpp>

namespace yq::doodler {
    class Py1Info : public PyInfo {
    public:
        template <typename T> class Writer;
        Py1Info(std::string_view zName, PyInfo& base, const std::source_location& sl=std::source_location::current());
    };
    
    

    /*! A physical object
    
        This represents a physical object, the number represents the number of coordinates needed to describe it.
    */
    class Py1 : public Py {
        YQ_OBJECT_INFO(Py1Info)
        YQ_OBJECT_DECLARE(Py1, Py)
    public:
    
        static void init_info();

    protected:
        Py1(Project&);
        Py1(Project&, const Py1&);
        ~Py1();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&);
        
        
    private:
        Py1(const Py1&) = delete;
        Py1(Py1&&) = delete;
        Py1& operator=(const Py1&) = delete;
        Py1& operator=(Py1&&) = delete;
    };

}
