////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/Py.hpp>

namespace yq::doodle {
    class Py4Info : public PyInfo {
    public:
        template <typename T> class Writer;
        Py4Info(std::string_view zName, const PyInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! A physical object
    
        This represents a physical object, the number represents the number of coordinates needed to describe it.
    */
    class Py4 : public Py {
        YQ_OBJECT_INFO(Py4Info)
        YQ_OBJECT_DECLARE(Py4, Py)
    public:
    
    protected:
        Py4(Project&);
        Py4(Project&, const Py4&);
        ~Py4();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Py4(const Py4&) = delete;
        Py4(Py4&&) = delete;
        Py4& operator=(const Py4&) = delete;
        Py4& operator=(Py4&&) = delete;
    };

    template <typename T>
    class Py4Info::Writer : public PyInfo::Writer<T> {
    public:
        Writer(Py4Info* pInfo) : PyInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(Py4Info& pInfo) : Writer(&pInfo)
        {
        }
    };

}
