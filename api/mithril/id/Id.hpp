////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/Flags.hpp>
#include <mithril/preamble.hpp>
#include <functional>
#include <memory>
#include <0/trait/always_false.hpp>

namespace yq::mithril {

    using IdTypeId      = uint16_t;
    using IdTypes       = Flags<IdTypeId, uint64_t>;

    namespace impl {
        template <typename T>
        concept HasIdMember    = requires(T a)
        {
            { a.id       } -> std::convertible_to<id_t>;
        };

        template <typename T>
        concept HasIDStatic     = requires
        {
            { T::ID } -> std::same_as<IdTypeId>;
        };
    }

    template <typename T>
    concept IdType      = impl::HasIdMember<T> || impl::HasIDStatic<T>;

    /*! \brief Unified identifier
    
        Given the size of the user-created data, it's reasonable to assume small
        data sets.  Therefore, this identifier couples a type with the numeric 
        identifier.  Uniqueness is preserved *UNTIL* an actual ID (in that type) 
        goes above the mask value, at which point, collisions will exist.
        
        This is needed for the Qt that restricts user data to 64-bits (and to avoid 
        a s-ton of special allocations).
    */
    struct Id {
    
        //! Tests to see if it's okay to blindly convert ID types
        static bool         compatible(IdTypeId from, IdTypeId to);
    
        //! Mask for the ID values
        static constexpr    uint64_t    MASK    = 0x00FF'FFFF'FFFF'FFFFULL;
        
        //! Shift for the type ID
        //! \note SHIFT and MASK must match to the ID type (currently 64 bits)
        static constexpr    uint64_t    SHIFT   = 56ULL;


        //! Mixes type & identifier to a single 64-bit value
        static constexpr uint64_t       encode(IdTypeId type, uint64_t id) noexcept
        {
            return (((uint64_t) type) << SHIFT) | (id & MASK);
        }

        //! Encodes from a specified type
        template <IdType S>
        static constexpr uint64_t       encode(S s) noexcept
        {
            return encode(S::ID, s.id);
        }

        //! Extracts the type-of from a value
        static constexpr IdTypeId        type_of(uint64_t v) noexcept
        {
            return (IdTypeId) (v >> SHIFT);
        }
        
        //! Extracts the numerical ID from a value
        static constexpr uint64_t       id_of(uint64_t v) noexcept
        {
            return v & MASK;
        }

        //! Decodes a value to an ID type
        template <IdType S>
        static S                        decode(uint64_t v) noexcept
        {
            return compatible(type_of(v), S::ID) ? S(id_of(v)) : S(0ULL);
        }

        //! Basic "semi-valid" test for a type and numeric identifier
        static constexpr bool           valid(uint64_t v) noexcept
        {
            return id_of(v) && type_of(v);
        }
        
        //! Name to Id Type
        //! \note may be empty
        static std::string_view         type_name(IdTypeId);
        
        //! TypeInfo for id type
        //! \note may come back NULL
        static const TypeInfo*          type_info(IdTypeId);
        
        //! Base types for id type
        static IdTypes                  base_types(IdTypeId);
        
        //! Highest valid type id
        static IdTypeId                 max_type();

        
        struct Repo;
        static Repo& repo();
        
        class  Lock;
        
    
        //  ..............................
    
        uint64_t            m_value  = 0ULL;
        
        constexpr Id() noexcept = default;
        
        template <IdType S>
        constexpr Id(S s) noexcept : m_value(encode(s)) {}
        
        explicit constexpr Id(uint64_t v) noexcept  : m_value(v) {}

        //! Type of the Id
        constexpr IdTypeId  type() const noexcept { return type_of(m_value); }
        
        //! Numeric identifier number
        constexpr uint64_t  id() const noexcept { return id_of(m_value); }
        
        //! Our value
        constexpr operator  uint64_t() const noexcept { return m_value; }
        
        //! Type name of this id
        std::string_view    type_name() const;
        
        //! Our key (db-call, might be empty)
        std::string         key() const;
        
        //! Our data's name (db-call, might be empty)
        std::string         name() const;

        //! Defaulted comparison operator
        constexpr auto operator<=>(const Id&) const noexcept = default;

        //! Extracts the ID to the specified type
        template <IdType S>
        S   as() const noexcept
        {
            return decode<S>(m_value);
        }
        
        //! Locking mechanism
        Lock        lock(bool rw=false) const;
    };



    using IdProvider    = std::function<std::vector<Id>()>;
    using IdFilter      = std::function<bool(Id)>;
 
    class Id::Lock {
    public:
    
        enum Mode {
            None = 0,
            Read,
            Write
        };
        
        Lock(){}
        Lock(Lock&&);
        Lock& operator=(Lock&&);
        
        ~Lock() { free(); }
        Mode        mode() const { return m_mode; }
        Id          id() const { return m_id; }
        operator bool () const noexcept { return m_mode != None; }
        void    free();

    private:
        Id      m_id;
        Mode    m_mode = None;
        
        friend struct Id;
        Lock(Id i, Mode m) : m_id(i), m_mode(m) {}
    };

    template <IdType C>
    std::set<id_t>                  ids_for(const std::set<C>& cs)
    {
        std::set<id_t> ret;
        for(auto& c : cs)
            ret.insert(c.id);
        return ret;
    }

    template <IdType C>
    std::vector<id_t>               ids_for(const std::vector<C>& cs)
    {
        std::vector<id_t> ret;
        for(auto& c : cs)
            ret.insert(c.id);
        return ret;
    }
    
    std::set<id_t>                  ids_for(const std::vector<const TypeInfo*>&);

    /*! \brief Blindly remaps a vector of id-objects
    */
    template <IdType A, IdType B>
    std::vector<B>  blind_remap(const std::vector<A>& old, B)
    {
        std::vector<B>  ret;
        ret.reserve(old.size());
        for(const A& a : old)
            ret.push_back(B{a.id});
        return ret;
    }

    template <IdType S>
    std::vector<Id> ids(const std::vector<S>& them)
    {
        return std::vector<Id>(them.begin(), them.end());
    }

    template <typename T>
    std::vector<Id> ids(const std::vector<T>& them)
    {
        static_assert(always_false_v<T>, "Argument is NOT an IdType");
        return {};
    }
}
