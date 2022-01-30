////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    //  Separating the header files since there's some interdependency issues.


#include <util/preamble.hpp>
#include <util/meta/DataBlock.hpp>

namespace yq {

    /*! \brief Generic Data

        A Variant is generic data, it's meant to be unobtrusive.  
    */
    class Variant {
    public:

        Variant();
        Variant(const Variant&);
        Variant(Variant&&);
        
        Variant(char);
        Variant(char8_t);
        Variant(char32_t);
        Variant(const char*);
        Variant(const char8_t*);
        Variant(const char32_t*);
        Variant(const std::string&);
        Variant(const std::u8string&);
        Variant(const std::u32string&);
        Variant(const std::wstring&);
        Variant(const String&);
    #ifdef ENABLE_QT
        Variant(const QString&);
    #endif
        
        
        //! Creates a defaulted Variant to the specified meta-type
        Variant(const meta::TypeInfo&);
        Variant(const meta::TypeInfo*);
        
        template <typename T>
        Variant(T&&);

        ~Variant();

        Variant&        operator=(const Variant&);
        Variant&        operator=(Variant&&);
        
        bool            operator==(const Variant&) const;
        
        //template <typename T>
        //bool        operator==(const T&) const;
        //template <typename T>
        //bool        operator!=(const T&) const;
        
        bool            can_convert_to(const meta::TypeInfo&) const;
        template <typename T>
        bool            can_convert_to() const;

        Variant         convert_to(const meta::TypeInfo&) const;

        template <typename T>
        Variant         convert_to() const;


        //Variant         get_field(const String&) const;
        
        String          io_format() const;
        static Variant  io_parse(const meta::TypeInfo&, const String&);
        
        bool            is_valid() const;
        
        
        /*! \brief "Printable" version for debugging/general-output
        
            \note There's no corresponding "parse" (deliberate), see the I/O helpers
        */
        String      print() const;
        
        //bool        set_field(const String&, const Variant&);

        const meta::TypeInfo& type() const { return *m_type; }

        template <typename T>
        Result<T>   value() const;
        
        Variant(const meta::TypeInfo&, const void*);

        const void*         ptr() const;
        
        void*               ptr();

    private:
        Variant(meta::TypeInfo&&) = delete;   // prohibt temporary metatypes

        const meta::TypeInfo*   m_type;
        impl::DataBlock     m_data;
        
        Variant(const meta::TypeInfo&&) = delete;
        
        
        template <typename T>
        void    set(T&&val);
    };
}


