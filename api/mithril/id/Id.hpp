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

    struct Id {
        static bool         compatible(IdTypeId from, IdTypeId to);
    
        static constexpr    uint64_t    MASK    = 0x0000'FFFF'FFFF'FFFFULL; // current max ID is 56-bits
        static constexpr    uint64_t    SHIFT   = 48ULL;

        static constexpr uint64_t       encode(IdTypeId type, uint64_t id) noexcept
        {
            return (((uint64_t) type) << SHIFT) | (id & MASK);
        }
        
        template <IdType S>
        static constexpr uint64_t       encode(S s) noexcept
        {
            return encode(S::ID, s.id);
        }

        static constexpr IdTypeId        type_of(uint64_t v) noexcept
        {
            return (IdTypeId) (v >> SHIFT);
        }
        
        static constexpr uint64_t       id_of(uint64_t v) noexcept
        {
            return v & MASK;
        }

        template <IdType S>
        static S                        decode(uint64_t v) noexcept
        {
            return compatible(type_of(v), S::ID) ? S(id_of(v)) : S(0ULL);
        }

        static constexpr bool           valid(uint64_t v) noexcept
        {
            return v && type_of(v);
        }
        
        static std::string_view         type_name(IdTypeId);
        static const TypeInfo*          type_info(IdTypeId);
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

        constexpr IdTypeId  type() const noexcept { return type_of(m_value); }
        constexpr uint64_t  id() const noexcept { return id_of(m_value); }
        constexpr operator  uint64_t() const noexcept { return m_value; }
        
        std::string_view    type_name() const;
        
        std::string         key() const;
        std::string         name() const;

        constexpr auto operator<=>(const Id&) const noexcept = default;
        
        template <IdType S>
        S   as() const noexcept
        {
            return decode<S>(m_value);
        }
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
