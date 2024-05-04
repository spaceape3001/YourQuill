////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/Py.hpp>

namespace yq::doodle {
    class Py3Info : public PyInfo {
    public:
        template <typename T> class Writer;
        Py3Info(std::string_view zName, const PyInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! A physical object
    
        This represents a physical object, the number represents the number of coordinates needed to describe it.
    */
    class Py3 : public Py {
        YQ_OBJECT_INFO(Py3Info)
        YQ_OBJECT_DECLARE(Py3, Py)
    public:
    
    protected:
        Py3(Project&);
        Py3(Project&, const Py3&);
        ~Py3();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        Py3(const Py3&) = delete;
        Py3(Py3&&) = delete;
        Py3& operator=(const Py3&) = delete;
        Py3& operator=(Py3&&) = delete;
    };

    template <typename T>
    class Py3Info::Writer : public PyInfo::Writer<T> {
    public:
        Writer(Py3Info* pInfo) : PyInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(Py3Info& pInfo) : Writer(&pInfo)
        {
        }
    };

}
