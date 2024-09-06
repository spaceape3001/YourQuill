////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/Py.hpp>
#include <yq-toolbox/typedef/vector4.hpp>
#include <yq-toolbox/unit/declare.hpp>

namespace yq::doodler {
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
    
        //! Position interface
        struct Position {
            virtual Vector4D    get_position() const = 0;
            virtual void        set_position(const Vector4D&) = 0;
        };

    protected:
        Py4(Project&);
        Py4(Project&, const Py4&);
        ~Py4();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&);
        
        
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
