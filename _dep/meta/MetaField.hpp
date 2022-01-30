#pragma once

#include "Meta.hpp"

namespace yq {

    class MetaValue;

    /*! \brief A read/writiable property to a value

        \note It's assumed fields are INDEPENDENT!
        \note For read-only, use Aspects
    */
    class MetaField : public Meta {
        friend class MetaValue;
        friend class Variant;
    public:

        //! MetaValue that this is a sub-to
        const MetaValue&        parent() const { return m_parent; }
        
        //! Type of the value
        const MetaValue&        value() const { return m_value; }
        
        struct Writer;
        virtual const char*     generic() const override { return "MetaField"; }
        
    protected:
        MetaField(const char*, MetaValue&, const MetaValue&);
        ~MetaField() {}

        virtual Variant         get(const void*) const = 0;
        virtual bool            set(void*, const void*) const = 0;

    private:

        MetaValue&              m_parent;
        const MetaValue&        m_value;
    };
}
