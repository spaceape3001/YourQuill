////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    //  Separating the header files since there's some interdependency issues.


#include <yq/preamble.hpp>
#include <yq/meta/DataBlock.hpp>
#include <yq/meta/Binder.hpp>

namespace yq {

    /*! \brief Generic Data

        A Variant is generic data, it's meant to be unobtrusive.  
    */
    class Variant {
    public:

        static Variant      parse_me(const TypeInfo&, const std::string_view&);

        Variant();
        Variant(const Variant&);
        Variant(Variant&&);
        
        //! Creates a defaulted Variant to the specified meta-type
        explicit Variant(const TypeInfo&);
        explicit Variant(const TypeInfo*);
        
        explicit Variant(char);
        explicit Variant(char8_t);
        explicit Variant(char32_t);
        explicit Variant(char*);
        explicit Variant(const char*);
        explicit Variant(const char8_t*);
        explicit Variant(const char32_t*);
        explicit Variant(const std::u8string&);
        explicit Variant(const std::u32string&);
        explicit Variant(const std::wstring&);
    #ifdef ENABLE_QT
        explicit Variant(const QString&);
    #endif

        /*! \brief Direct construction constructor
        
            This directly creates a variant with the argument.
            
            \note EXPLICIT is required to keep the compiler from getting greedy.
        */
        template <typename T>
        explicit Variant(T&&);

        ~Variant();

        Variant&        operator=(const Variant&);
        Variant&        operator=(Variant&&);
        
        template <typename T>
        requires is_type_v<std::decay_t<T>>
        Variant&        operator=(T&&);
        
        bool            operator==(const Variant&) const;
        
        template <typename T>
        requires is_type_v<T>
        bool        operator==(const T&) const;
        //template <typename T>
        //bool        operator!=(const T&) const;
        
        bool            can_convert(const TypeInfo&) const;
        template <typename T>
        bool            can_convert() const;

        //! Returns a variant that's been converted
        Variant         convert(const TypeInfo&) const;

        template <typename T>
        Variant         convert() const;


        bool            is_valid() const;

        //Variant         get_field(const std::string&) const;
        
        /*! \brief Parses into the variant, overwriting
        */
        bool             parse(const TypeInfo&, const std::string_view&);
        
        
        /*! \brief Parses into the variant, overwriting
        
            Type info is assumed to be STRING if variant is invalid.
        */
        bool             parse(const std::string_view&);
        
        
        
        /*! \brief Debug writing to stream
        
            This is meant for printing casually to the screen for the user's benefit (debugging, alerts, etc),
            therefore, we'll opt for being concise over precision (ie. missing lesser bits is alright).
        */
        bool            print(Stream&) const;
        
        
        /*! \brief "Printable" version for debugging/general-output
        
            \note There's no corresponding "parse" (deliberate), see the I/O helpers
        */
        std::string          printable() const;
        
        /*! \brief Guarded pointer
            Returns the pointer *IF* it's feasible, otherwise null
        */
        template <typename T>
        T*              ptr();

        /*! \brief Guarded pointer
            Returns the pointer *IF* it's feasible, otherwise null
        */
        template <typename T>
        const T*        ptr() const;

        
        //! Raw pointer to void
        const void*         raw_ptr() const;
        //! Raw pointer to void
        void*               raw_ptr();
        
        template <typename T>
        const T&            ref(const T& bad={}) const;
        

        //bool        set_field(const std::string&, const Variant&);

        

        const TypeInfo& type() const { return *m_type; }

        template <typename T>
        Result<const T&>    value() const;
        
        /*! \brief IO Writing to stream
        
            This is meant to write the data out in a way that'll fully capture the input (ie better to have a thousand 
            decimals over missing a bit).  If not present and it's a compound object, then the algorithm driving this
            ought to delve deeper.
            
            \brief TRUE if properly delegated
        */
        bool            write(Stream&) const;


        /*! \brief The "I-KNOW-WHAT-I'M-DOING" constructor
        
            This routine blindly assumes the caller knows what they're doing, so here's the type and here's a pointer
            to the data.  A null pointer will force a default construction.
        */
        Variant(const TypeInfo&, const void*);

        const DataBlock&    data() const { return m_data; }

    private:
        Variant(TypeInfo&&) = delete;   // prohibt temporary metatypes

        const TypeInfo*     m_type;
        DataBlock           m_data;
        
        Variant(const TypeInfo&&) = delete;
        
        
        template <typename T>
        void    set(T&&val);
        
        void    set(std::string_view&&);
        void    set(const std::string_view&);
        
        void    set(const TypeInfo&, const void*);
        void    clear();
        
        //! Checks to make sure the type info is populated to the minimum specification
        static bool    good(const TypeInfo&);
        
    };
}


