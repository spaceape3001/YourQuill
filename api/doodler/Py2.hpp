////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/Py.hpp>
#include <yq/typedef/vector2.hpp>
#include <yq-toolbox/unit/declare.hpp>

namespace yq::doodler {
    class Py2Info : public PyInfo {
    public:
        template <typename T> class Writer;
        Py2Info(std::string_view zName, const PyInfo& base, const std::source_location& sl=std::source_location::current());
    };
    

    /*! A physical object
    
        This represents a physical object, the number represents the number of coordinates needed to describe it.
    */
    class Py2 : public Py {
        YQ_OBJECT_INFO(Py2Info)
        YQ_OBJECT_DECLARE(Py2, Py)
    public:
    
        //! Position interface
        struct Position {
            virtual Vector2D    get_position() const = 0;
            virtual void        set_position(const Vector2D&) = 0;
        };
    
        //! Orientation interface
        struct Orientation {
            virtual Radian      get_orientation() const = 0;
            virtual void        set_orientation(Radian) = 0;
        };
    
    
    protected:
        Py2(Project&);
        Py2(Project&, const Py2&);
        ~Py2();

        //! Remap IDs/pointers appropriately
        virtual void        remap(const Remapper&);
        
        
    private:
        Py2(const Py2&) = delete;
        Py2(Py2&&) = delete;
        Py2& operator=(const Py2&) = delete;
        Py2& operator=(Py2&&) = delete;
    };

    template <typename T>
    class Py2Info::Writer : public PyInfo::Writer<T> {
    public:
        Writer(Py2Info* pInfo) : PyInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(Py2Info& pInfo) : Writer(&pInfo)
        {
        }
    };

}
