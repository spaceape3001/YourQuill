#pragma once

#include "StringsFwd.hpp"
#include <algorithm>

namespace yq {

    class StringView : public std::string_view {
    public:

        class Iter32;
        class IterW;

        static constexpr std::string_view     ctor_guard(const std::string_view& s, size_t i, size_t sz)
        {
            if(i >= s.size())
                return std::string_view();
            return std::string_view(s.data()+i, std::min(s.size()-i, sz));
        }


        constexpr StringView(){}
        
        constexpr StringView(const StringView&) = default;
        constexpr StringView(const std::string_view& cp) : std::string_view(cp) {}
        constexpr StringView(StringView&&) = default;
        constexpr StringView(std::string_view&&mv) : std::string_view(std::move(mv)) {}
        
        constexpr StringView(const char*s) : std::string_view(s) {}
        constexpr StringView(const char*s, size_t sz) : std::string_view(s,sz) {}
        constexpr StringView(const char*s, size_t i, size_t sz) : std::string_view(s+i,sz) {}
        constexpr StringView(const std::string_view&s, size_t sz) : std::string_view(s.data(), std::min(sz, s.size()))  {}
        constexpr StringView(const std::string_view&s, size_t i, size_t sz) : std::string_view(ctor_guard(s,i,sz)) {}
        constexpr StringView(const StringView&s, size_t sz) : std::string_view(s.data(), std::min(sz, s.size()))  {}
        constexpr StringView(const StringView&s, size_t i, size_t sz) : std::string_view(ctor_guard(s,i,sz)) {}
        
        template <class It, class End>
        constexpr StringView( It first, End last ) : std::string_view(first, last) {}
        
        constexpr StringView& operator=(const StringView&) = default;
        constexpr StringView& operator=(const std::string_view&cp) 
        {
            std::string_view::operator=(cp);
            return *this;
        }
        
        constexpr StringView& operator=(const char* cp) 
        {
            std::string_view::operator=(cp);
            return *this;
        }
        
        ~StringView(){}

        size_t              char_count() const;
        
        size_t              count_of(char32_t) const;

        template <typename Pred>
        bool                for_each(Pred p) const;

        template <typename Pred>
        bool                for_each_abort(Pred p) const;

        template <typename Pred>
        bool                for_each_w(Pred p) const;

        Iter32              iterate() const;
        IterW               iterate_w() const;

        bool                matches_at(const StringView&, size_t pos=0) const;

        QByteArray          qBytes() const;
        QString             qString() const;
        
        Vector<StringView>  split(char ch) const;
        Vector<StringView>  split(char ch, unsigned int) const;
        Vector<StringView>  split(const char*z) const;
        Vector<StringView>  split(const char*z,unsigned int) const;
        Vector<StringView>  split(const std::string& s) const;
        Vector<StringView>  split(const std::string& s,unsigned int) const;
        Vector<StringView>  split(const std::string_view& s) const;
        Vector<StringView>  split(const std::string_view& s,unsigned int) const;

        bool                starts_with(const std::string&) const;
        bool                starts_with(const std::string_view&) const;
        bool                starts_with(const char*) const;

        
            //  these to util only....
        boolean_r           to_boolean() const;
        double_r            to_double() const;
        float_r             to_float() const;
        unsigned_r          to_hex() const;
        uint8_r             to_hex8() const;
        uint16_r            to_hex16() const;
        uint32_r            to_hex32() const;
        uint64_r            to_hex64() const;
        int8_r              to_int8() const;
        int16_r             to_int16() const;
        int32_r             to_int32() const;
        int64_r             to_int64() const;
        integer_r           to_integer() const;
        short_r             to_short() const;
        std::u8string       to_u8string() const;
        u32string_r         to_u32string() const;
        uint8_r             to_uint8() const;
        uint16_r            to_uint16() const;
        uint32_r            to_uint32() const;
        uint64_r            to_uint64() const;
        unsigned_r          to_uinteger() const;
        ushort_r            to_ushort() const;
        wstring_r           to_wstring() const;
    };



    /*! \brief Used to iterate through the string as char32_t
        \note This stores a REFERENCE to the string!  and do NOT modify he string
    */
    class StringView::Iter32 {
    public:
        Iter32(const std::string& s) : Iter32(s.data(), s.size()) {}
        Iter32(const std::string_view& s) : Iter32(s.data(), s.size()) {}
        Iter32(const char*);
        Iter32(Iter32&& mv) = default;
        
        Iter32&   operator=(Iter32&&) = default;
        
        Char32      next();
        size_t      position() const;
        bool        done() const { return m_data >= m_end; }
        bool        more() const { return (m_data < m_end) && !m_error; }
        bool        error() const { return m_error; }
        
    private:
        constexpr Iter32(const char*z, size_t n) :
            m_begin(z), 
            m_data(z), 
            m_end(z+n), 
            m_state{},
            m_error(false)
        {
        }
        
        Iter32(std::string&&) = delete;
        Iter32(std::string_view&&) = delete;
        Iter32(const Iter32&) = delete;
        Iter32& operator=(const Iter32&) = delete;
        
        const char*     m_begin;
        const char*     m_data;
        const char*     m_end;
        std::mbstate_t  m_state;
        bool            m_error;
    };

    /*! \brief Stores a REFERENCE to the string!
    */
    class StringView::IterW {
    public:

        IterW(const std::string& s);
        IterW(const std::string_view&);
        IterW(const char*);
        IterW(IterW&&);

        wchar_t     next();
        size_t      position() const;
        bool        done() const { return m_data >= m_end; }
        bool        more() const { return (m_data < m_end) && !m_error; }
        bool        error() const { return m_error; }

    private:
        IterW(const char*, size_t);
        IterW(std::string&&) = delete;
        IterW(std::string_view&&) = delete;
        IterW(const Iter32&) = delete;
        
        const char*     m_begin;
        const char*     m_data;
        const char*     m_end;
        std::mbstate_t  m_state;
        bool            m_error;
    };

    template <typename Pred>
    bool            StringView::for_each(Pred pred) const
    {
        const char* p   = data();
        const char* e   = p + size();
        std::mbstate_t  state{};
        char32_t     wc = 0;
        int          len = 0;

        while((p < e) && ((len = std::mbrtoc32(&wc, p, e-p, &state)) > 0)){
            pred(Char32(wc));
            p += len;
        }
        return p == e;
    }

    template <typename Pred>
    bool            StringView::for_each_abort(Pred pred) const
    {
        const char* p   = data();
        const char* e   = p + size();
        std::mbstate_t  state{};
        char32_t     wc = 0;
        int          len = 0;

        while((p < e) && ((len = std::mbrtoc32(&wc, p, e-p, &state)) > 0)){
            if(!pred(Char32(wc)))
                break;
            p += len;
        }
        return p == e;
    }


    template <typename Pred>
    bool            StringView::for_each_w(Pred pred) const
    {
        const char* p   = data();
        const char* e   = p + size();
        std::mbstate_t  state{};
        wchar_t     wc = 0;
        int         len = 0;

        while((p < e) && ((len = std::mbrtowc(&wc, p, e-p, &state)) > 0)){
            pred(wc);
            p += len;
        }
        return p == e;
    }
}
