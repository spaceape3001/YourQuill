////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MetaFwd.hpp"
#include "util/Map.hpp"
#include "util/Set.hpp"
#include "util/String.hpp"

class Variant;
class MetaWriter;

/*! \brief Root "meta" info for all meta info, everything else will be derived from this

    \note We will be ASSUMING single threaded until core_freeze() is called!  After that, everything wil be treated
    as READ ONLY.
*/
class Meta {
protected:
    static constexpr const unsigned int     iBAD    = ~(unsigned int) 0;
    enum {
        isPointer       = 1 << 0,
        isObject        = 1 << 1,
        isStruct        = 1 << 2,
        isType          = 1 << 3,
        isValue         = 1 << 4,
        isField         = 1 << 5,
        isGlobal        = 1 << 6,
        isProperty      = 1 << 7,
        isFunction      = 1 << 8,
        isAspect        = 1 << 9,
        isDelegate      = 1 << 10
    };
public:

    static const Vector<const Meta*>&       all();
    static const Meta*                      lookup(unsigned int);


    /*! \brief Any relavant aliases.
    */
    const Set<String>&                      aliases() const { return m_aliases; }
    const String&                           description() const { return m_description; }
    const String&                           label() const { return m_label; }
    const String&                           name() const { return m_name; }

    unsigned int                            id() const { return m_id; }
    
    bool                                    is_delegate() const { return (m_type & isDelegate) != 0; }
    bool                                    is_field() const { return (m_type & isField) != 0; }
    bool                                    is_function() const { return (m_type & isFunction) != 0; }
    bool                                    is_global() const { return (m_type & isGlobal) != 0; }
    bool                                    is_object() const { return (m_type & isObject) != 0; }
    bool                                    is_property() const { return (m_type & isProperty) != 0; }
    bool                                    is_type() const { return (m_type & isType) != 0; }
    bool                                    is_value() const { return (m_type & isValue) != 0; }

    bool                                    has_tag(const String&) const;
    const Variant&                          tag(const String&) const;

    /*! Tags key/value
    */
    void    tag(const String&, const Variant&);
    
    virtual const char* generic() const { return "Meta"; }

protected:


    explicit Meta(const String&, unsigned int i=iBAD);
    virtual ~Meta();    // do NOT delete.... C++ won't let me delete this

    void    alias(const String&);
    void    description(const String&);
    void    label(const String&);
    void    name(const String&);
    
private:

    friend class MetaWriter;

    //~Meta() = delete;
    Meta(const Meta&) = delete;
    Meta(Meta&&) = delete;
    void operator=(const Meta&) = delete;
    void operator=(Meta&&) = delete;
    using TagMap = Map<String,Variant*,IgCase>;

    Set<String>         m_aliases;
    String              m_description;
    String              m_label;
    String              m_name;
    TagMap              m_tags;
    unsigned int        m_id;
    
protected:

    //  TBD
    uint64_t            m_type;
    
};
