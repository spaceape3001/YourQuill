////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/collection/Reverse.hpp>
#include <util/collection/Set.hpp>
#include <util/collection/Vector.hpp>
#include <util/text/Utils.hpp>
#include <util/type/Result.hpp>
#include <string>
#include <string_view>

namespace yq {
    struct KeyValue;


    /*! \brief Collection of key-value pairs

        Key Values are stored as a tree.
    */
    struct KVTree {

        enum {
            RECURSIVE   = 0x1,
            BODY        = 0x2
        };

        //! Initializer of const char*, used for faster declarations
        using Seq               = const std::initializer_list<const char*>;

        //! \brief Sub-KeyValues
        //! These are the sub-key-values (honored for recursive key-values)
        Vector<KeyValue>        subs;
        
        //! Finds all matches, returns a vector
        template <typename Match>
        Vector<const KeyValue*>     all(Match) const;

        //! Finds all matches in visitor pattern
        template <typename Match, typename Pred>
        void                        all(Match, Pred) const;

        //! \brief Is empty?  
        //! This checks the strings/subkey-values for emptyness and returns appropriately
        bool                        empty() const { return subs.empty(); }
        
        //! \brief Erases ALL key-values that match
        template <typename Match>
        void                        erase_all(Match);
        
        //! \brief Erases ALL key-values that match
        template <typename Match>
        void                        erase_seconds(Match);
        
        //! \brief Returns the FIRST attribute
        //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
        template <typename Match>
        KeyValue*                  first(Match);
        
        //KeyValue*                   first(const char*);
        //KeyValue*                   first(const std::string_view&);
        //KeyValue*                   first(const std::string&);
        KeyValue*                   first(std::initializer_list<const char*>);

        //! \brief Returns the FIRST attribute
        //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
        template <typename Match>
        const KeyValue*             first(Match) const;

       
        const KeyValue*             first(std::initializer_list<const char*>) const;
        void                        fusion(const KVTree&);
        
        //! \brief Checks for a match
        template <typename Match>
        bool                        has(Match) const;
        
        //! \brief Returns the LAST attribute
        //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
        template <typename Match>
        const KeyValue*            last(Match) const;

        size_t                     max_key_size(bool recursive=false) const;
        
        //! \brief Sets the specified attribute
        //!
        //!  This sets the first attribute encountered to the specified value,   If the attribute does not exist, then
        //!  it will be created.
        //!
        //! \note WARNING, this will (possibly) modify the vector, consider existing pointers to be invalid
        //!
        //! \param  m       Match
        //! \param  key     Key to add (if not found)
        //! \param  data    New value
        //! \param  purge   If true, deletes all OTHER attirubtes with the same key
        //! \return Pointer to the attribute
        template <typename Match>
        KeyValue*                  set(Match, const std::string_view&key, const std::string_view& data, bool purge=true);

        KeyValue*                  set(const std::string_view&key, const std::string_view& data, bool purge=true);
        KeyValue*                  set(const string_view_vector_t& keys, const std::string_view& data, bool purge=true);
        KeyValue*                  set(const std::initializer_list<const char*>& keys, const std::string_view& data, bool purge=true);
        

        template <typename Match, typename T>
        KeyValue*                  set(Match, const std::string_view&key, const T& data, bool purge=true);
        
        //  being jettisoned  set_set() ... use "join" instead.
        
        
        //! \brief First value for string, empty if not found (or empty w/in)
        template <typename Match>
        std::string_view            value(Match) const;
        
        //std::string_view            value(const char*) const;
        //std::string_view            value(const std::string&) const;
        //std::string_view            value(const std::string_view&) const;
        std::string_view            value(std::initializer_list<const char*>) const;
        
        
        //! \brief All non-empty values for string
        template <typename Match>
        string_view_vector_t        values(Match) const;
        
        //! \brief All non-empty values for string
        template <typename Match, typename Pred>
        void                        values(Match, Pred) const;

        template <typename Match>
        string_view_set_t           values_set(Match, const std::string_view& sep=",") const;
        
        template <typename Match>
        Set<uint16_t>               values_set_u16(Match, const std::string_view& sep=",") const;
        
        KVTree&                     operator+=(KVTree&&);
        KVTree&                     operator+=(const KVTree&);
        KVTree&                     operator+=(KeyValue&&);
        KVTree&                     operator+=(const KeyValue&);
        
        KVTree&                     operator<<(KeyValue&&);
        KVTree&                     operator<<(const KeyValue&);
        
        bool                        parse(const ByteArray& buffer, std::string* body, bool recursive, const std::string_view& fname);
        //void                        write(Vector<char>&) const;
        void                        write(yq::Stream&) const;
    };

    //! \brief KeyValue (in a file)
    //! 
    //! This is a generic attribute, expected to be a key/value nature.  It has several other features to allow for
    //! reasonable data fusion.  It's nature is recursive, however, whether it is or not is up to the specific use-case.
    struct KeyValue : public KVTree {
        //! \brief Command
        //! This is the optional command for the attribute.  It's intended for add/remove/modify/comment/etc
        //! However, '%' is reserved for system defined attribute keys
        std::string              cmd;
        
        //! \brief KeyValue Identifier
        //! This is the identifier for the attribute, it'll be surrounded by [] in the file and is expected to be
        //! alpha-numeric in nature.  Limited punctuation acceptable, spaces ignored on read
        std::string              id;
        
        //! \brief KeyValue Key
        //! This is the key, and is expected to present in everything except a comment.  Keys are expected to be
        //! Alpha-numeric in nature.  Limited punctuation acceptable, spaces will trip the value
        std::string              key;
        
        //! \brief KeyValue Value/data
        //! This is the attribute's valueData.  If the command isa comment, then this is the contents of that comment.
        std::string              data;

        //! \brief Indent of the attribute
        //! Ignored on write, this is the original indentation value for the attribute from the file.
        unsigned int        indent = 0;


        KeyValue() = default;
        KeyValue(const std::string_view& k) : key(k) {}
        KeyValue(const std::string_view& k, const std::string_view& v) : key(k), data(v) {}
        
        bool                empty() const;
        
    };
    

    namespace kv {
    
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  MATCH ON KEYS
    
        struct SingleKeyMatch {
            std::string_view        pat;
            bool operator()(const KeyValue& a) const
            {
                return is_in(a.key, pat);
            }
        };
        
            //! \brief Match the key
        inline SingleKeyMatch        key(const std::string_view& s) { return SingleKeyMatch{s}; }

            //  ------------------------------------------------------------------------------------------------

        struct InitKeyMatch {
            const std::initializer_list<std::string_view>*   pat = nullptr;
            bool operator()(const KeyValue& a) const
            {
                return is_in(a.key, *pat);
            }
        };
        
        inline InitKeyMatch          key(const std::initializer_list<std::string_view>& s) { return InitKeyMatch(&s); }

            //  ------------------------------------------------------------------------------------------------

        struct VectorKeyMatch {
            const std::vector<std::string_view>*   pat = nullptr;
            bool operator()(const KeyValue& a) const
            {
                return is_in(a.key, *pat);
            }
        };

        inline VectorKeyMatch        key(const std::vector<std::string_view>& s) { return VectorKeyMatch(&s); }

            //  ------------------------------------------------------------------------------------------------

        struct CopyVectorKeyMatch {
            std::vector<std::string_view>   pat;
            bool operator()(const KeyValue& a) const
            {
                return is_in(a.key, pat);
            }
        };
        
        inline CopyVectorKeyMatch        key(const std::initializer_list<const char*>& keys) { 
            CopyVectorKeyMatch ret;
            for(auto& k : keys)
                ret.pat.push_back(k);
            return ret;
        }
        
            //  ------------------------------------------------------------------------------------------------

        struct SetKeyMatch {
            const string_view_set_t*    pat = nullptr;
            bool operator()(const KeyValue& a) const
            {
                return is_in(a.key, *pat);
            }
        };
        
        inline SetKeyMatch      key(const string_view_set_t& s) { return SetKeyMatch(&s); }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  MATCH ON KEYS W/O COMMAND
    
        struct SingleNonCmdKeyMatch {
            std::string_view        pat;
            bool operator()(const KeyValue& a) const
            {
                return a.cmd.empty() && is_in(a.key, pat);
            }
        };
        
        inline SingleNonCmdKeyMatch noncmd_key(const std::string_view& s) { return SingleNonCmdKeyMatch{s}; }

            //  ------------------------------------------------------------------------------------------------

        struct InitNonCmdKeyMatch {
            const std::initializer_list<std::string_view>*    pat = nullptr;
            bool operator()(const KeyValue& a) const
            {
                return a.cmd.empty() && is_in(a.key, *pat);
            }
        };
        
        inline InitNonCmdKeyMatch   noncmd_key(const std::initializer_list<std::string_view>& s) { return InitNonCmdKeyMatch(&s); }

            //  ------------------------------------------------------------------------------------------------

        struct VectorNonCmdKeyMatch {
            const std::vector<std::string_view>*    pat = nullptr;
            bool operator()(const KeyValue& a) const
            {
                return a.cmd.empty() && is_in(a.key, *pat);
            }
        };

        inline VectorNonCmdKeyMatch noncmd_key(const std::vector<std::string_view>& s) { return VectorNonCmdKeyMatch(&s); }

            //  ------------------------------------------------------------------------------------------------

        struct SetNonCmdKeyMatch {
            const string_view_set_t*    pat = nullptr;
            bool operator()(const KeyValue& a) const
            {
                return a.cmd.empty() && is_in(a.key, *pat);
            }
        };
        
        inline SetNonCmdKeyMatch      noncmd_key(const string_view_set_t& s) { return SetNonCmdKeyMatch(&s); }
    }
        
    template <typename Match>
    Vector<const KeyValue*>  KVTree::all(Match m) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return all( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return all( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return all( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return all( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return all( kv::key(m) );
        } else {
            Vector<const KeyValue*> ret;
            for(const KeyValue& a : subs)
                if(m(a))
                    ret << &a;
            return ret;
        }
    }
    

    template <typename Match, typename Pred>
    void  KVTree::all(Match m, Pred pred) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            all( kv::key(m), pred );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            all( kv::key(m), pred );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            all( kv::key(m), pred );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            all( kv::key(m), pred );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            all( kv::key(m), pred );
        } else {
            for(const KeyValue& a : subs)
                if(m(a))
                    pred(a);
        }
    }
    
    template <typename Match>
    void  KVTree::erase_all(Match m)
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            erase_all( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            erase_all( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            erase_all( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            erase_all( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            erase_all( kv::key(m) );
        } else {
            subs.erase_if(m);
        }
    }

    template <typename Match>
    void  KVTree::erase_seconds(Match m)
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            erase_seconds( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            erase_seconds( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            erase_seconds( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            erase_seconds( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            erase_seconds( kv::key(m) );
        } else {
            //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
            bool    f   = false;
            subs.erase_if([&](const KeyValue& a) -> bool {
                if(m(a)){
                    if(f)
                        return true;
                    f   = true;
                }
                return false;
            });
        }
    }

    template <typename Match>
    KeyValue*  KVTree::first(Match m)
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return first( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return first( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return first( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return first( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return first( kv::key(m) );
        } else {
            for(KeyValue& a : subs)
                if(m(a))
                    return &a;
            return nullptr;
        }
    }

    template <typename Match>
    const KeyValue*  KVTree::first(Match m) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return first( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return first( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return first( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return first( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return first( kv::key(m) );
        } else {
            for(const KeyValue& a : subs)
                if(m(a))
                    return &a;
            return nullptr;
        }
    }

    template <typename Match>
    bool  KVTree::has(Match m) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return has( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return has( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return has( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return has( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return has( kv::key(m) );
        } else {
            return first(m) != nullptr;
        }
    }

    template <typename Match>
    const KeyValue*  KVTree::last(Match m) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return last( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return last( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return last( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return last( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return last( kv::key(m) );
        } else {
            for(const KeyValue& a : reverse(subs))
                if(m(a))
                    return &a;
            return nullptr;
        }
    }

    template <typename Match>
    KeyValue*       KVTree::set(Match m, const std::string_view&key, const std::string_view& data, bool purge)
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return set( kv::key(m), key, data, purge );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return set( kv::key(m), key, data, purge );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return set( kv::key(m), key, data, purge );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return set( kv::key(m), key, data, purge );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return set( kv::key(m), key, data, purge );
        } else {
            if(purge)
                erase_seconds(m);
                
            KeyValue* a = first(m);
            if(a){
                a -> data   = data;
                return a;
            }
            
            KeyValue    n;
            n.key       = key;
            n.data      = data;
            subs.push_back(n);
            return &subs.back();
        }
    }
    
    template <typename Match, typename T>
    KeyValue*  KVTree::set(Match m, const std::string_view&key, const T& data, bool purge)
    {
        auto    sd  = to_string(data);  // here to anchor the results until AFTER set
        return set(m, key, std::string_view(sd), purge);
    }
    
    template <typename Match>
    std::string_view  KVTree::value(Match m) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return value( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return value( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return value( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return value( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return value( kv::key(m) );
        } else {
            const KeyValue* a = first(m);
            return a ? a->data : std::string_view();
        }
    }
    
    
    template <typename Match>
    string_view_vector_t  KVTree::values(Match m) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return values( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return values( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return values( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return values( kv::key(m) );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return values( kv::key(m) );
        } else {
            string_view_vector_t  ret;
            all(m, [&](const KeyValue& a){
                if(!a.data.empty())
                    ret << a.data;
            });
            return ret;
        }
    }
    
    template <typename Match, typename Pred>
    void  KVTree::values(Match m, Pred pred) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            values( kv::key(m), pred );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            values( kv::key(m), pred );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            values( kv::key(m), pred );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            values( kv::key(m), pred );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            values( kv::key(m), pred );
        } else {
            all(m, [&](const KeyValue& a){
                if(!a.data.empty())
                    pred(a.data);
            });
        }
    }

    template <typename Match>
    string_view_set_t KVTree::values_set(Match m, const std::string_view& sep) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return values_set( kv::key(m), sep );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return values( kv::key(m), sep );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return values( kv::key(m), sep );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return values( kv::key(m), sep );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return values( kv::key(m), sep );
        } else {
            string_view_set_t   ret;
            all(m, [&](const KeyValue&a){
                if(!a.data.empty()){
                    vsplit(a.data, sep, [&](const std::string_view& v){
                        if(!v.empty())
                            ret << v;
                    });
                }
            });
            return ret;
        }
    }
    
    template <typename Match>
    Set<uint16_t> KVTree::values_set_u16(Match m, const std::string_view& sep) const
    {
        if constexpr ( std::is_same_v<Match, const char*>) {
            return values_set_u16( kv::key(m), sep );
        } else if constexpr ( std::is_same_v<Match, std::string_view>) {
            return value( kv::key(m), sep );
        } else if constexpr ( std::is_same_v<Match, std::string>) {
            return values_set_u16( kv::key(m), sep );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<std::string_view>>) {
            return values_set_u16( kv::key(m), sep );
        } else if constexpr ( std::is_same_v<Match, std::initializer_list<const char*>>) {
            return values_set_u16( kv::key(m), sep );
        } else {
            Set<uint16_t>   ret;
            for(const std::string_view& v : values_set(m, sep)){
                auto r = to_uint16(v);
                if(r.good)
                    ret << r.value;
            }
            return ret;
        }
    }


    
}
