////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/DObject.hpp>

namespace yq::doodle {
    class PyInfo : public DObjectInfo {
    public:
        template <typename T> class Writer;
        PyInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! A physical object
    
        This represents a physical object, the number represents the number of coordinates needed to describe it.
    */
    class Py : public DObject {
        YQ_OBJECT_INFO(PyInfo)
        YQ_OBJECT_DECLARE(Py, DObject)
    public:
    
    protected:
        Py(Project&);
        Py(Project&, const Py&);
        ~Py();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Py(const Py&) = delete;
        Py(Py&&) = delete;
        Py& operator=(const Py&) = delete;
        Py& operator=(Py&&) = delete;
    };

    template <typename T>
    class PyInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(PyInfo* pInfo) : DObjectInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(PyInfo& pInfo) : Writer(&pInfo)
        {
        }
    };

}
