////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/delegate/StringDelegate.hpp>
#include <gui/delegate/EnumDelegate.hpp>
#include <util/TypeFwd.hpp>
#include <util/TypeTraits.hpp>
#include <QValidator>
#include <QVariant>

template <typename> class StdTableModel;
template <typename> struct StdColumnWriter;

template <typename T>
class StdAdder {
public:
    const QVariant&     message() const { return m_message; }
    virtual Result<T>   make(const QVariant&) = 0;
    virtual ~StdAdder(){}
protected:
    StdAdder(const QVariant&m) : m_message(m) {}
private:
    QVariant            m_message;
};

template <typename T>
class StdDelegator {
public:
    virtual Delegate*   create(QObject*)  = 0;
    virtual ~StdDelegator(){}
};

template <typename T, class D>
class SDMSimple : public StdDelegator<T> {
public:
    virtual ~SDMSimple(){}
    virtual Delegate*   create(QObject*par) override
    {
        return new D(par);
    }
};

template <typename T, class D>
class SDMValidator : public StdDelegator<T> {
public:
    ~SDMValidator(){}
    virtual Delegate*   create(QObject* par) override
    {
        return new StringDelegate( new D(par), par);
    }
};



template <typename T>
class StdColumn {
public:

    StdAdder<T>*        adder() const { return m_add.get(); }

    Delegate*           delegate(QObject*obj) const
    {
        return m_delegate ? m_delegate->create(obj) : nullptr;
    }

    int                 id() const { return m_id; }
    bool                settable() const { return m_settable; }
    const String&       key() const { return m_key; }
    const QVariant&     label() const { return m_label; }

    virtual QVariant    display(const T&) const = 0;
    virtual QVariant    edit(const T&v) const { return display(v); }
    virtual QVariant    decoration(const T&) const { return QVariant(); }
    virtual QVariant    toolTip(const T&) const { return QVariant(); }
    virtual QVariant    statusTip(const T&) const { return QVariant(); }
    virtual QVariant    whatsThis(const T&) const { return QVariant(); }
    virtual QVariant    sizeHint(const T&) const { return QVariant(); }
    virtual QVariant    font(const T&) const { return QVariant(); }
    virtual QVariant    textAlign(const T&) const { return QVariant(); }
    virtual QVariant    background(const T&) const { return QVariant(); }
    virtual QVariant    foreground(const T&) const { return QVariant(); }
    virtual QVariant    accText(const T&) const { return QVariant(); }
    virtual QVariant    accDesc(const T&) const { return QVariant(); }
    virtual bool        set(T&, const QVariant&) const { return false; }
    
    bool                sortable() const { return m_sortable; }
    virtual bool        less(const T&, const T&) const { return false; }
    virtual ~StdColumn(){}
    
protected:
    
    StdColumn(const String&k) : m_key(k), m_label(k.qString()), m_id(-1), m_settable(false), m_sortable(false)
    {
    }
    
    friend class StdTableModel<T>;
    friend struct StdColumnWriter<T>;
    
    std::unique_ptr<StdAdder<T>>      m_add;
    std::unique_ptr<StdDelegator<T>>  m_delegate;
    String                      m_key;
    QVariant                    m_label;
    int                         m_id;
    bool                        m_settable;
    bool                        m_sortable;

};

template <typename T>
struct StdColumnWriter {
    StdColumnWriter&   add(StdAdder<T>* a)
    {
        if(m_column)
            m_column -> m_add.reset(a);
        return *this;
    }
    
    StdColumnWriter&   label(const QVariant& v)
    {
        if(m_column)
            m_column -> m_label = v;
        return *this;
    }
    
    StdColumnWriter&   readOnly() 
    {
        if(m_column)
            m_column -> m_settable  = false;
        return *this;
    }
    
    StdColumnWriter&   delegate(StdDelegator<T>* d)
    {
        if(m_column)
            m_column -> m_delegate.reset(d);
        return *this;
    }
    
    StdColumnWriter&   unsortable()
    {
        if(m_column)
            m_column -> m_sortable  = false;
        return *this;
    }
    

    template <class D>
    StdColumnWriter&  delegate()
    {
        if(m_column){
            if constexpr (std::is_base_of_v<Delegate, D>){
                m_column -> m_delegate.reset(new SDMSimple<T,D>);
            } else if constexpr(std::is_base_of_v<QValidator, D>){
                m_column -> m_delegate.reset(new SDMValidator<T,D>);
            }
        }
        return *this;
    }
    
    
    StdColumn<T>* m_column;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Simple String member variable column
template <typename T>
struct SCMStringMV : public StdColumn<T> {
    typedef String  (T::*Ptr);
    Ptr                 m_ptr;
    bool                m_cs;

    SCMStringMV(const String&k, Ptr p, bool cs) : StdColumn<T>(k), m_ptr(p), m_cs(cs) 
    {
        StdColumn<T>::m_sortable  = true;
    }
    
    ~SCMStringMV(){}

    QVariant    display(const T&d) const override
    {
        return (d.*m_ptr).qString();
    }
    
    bool        set(T& d, const QVariant& v) const override
    {
        d.*m_ptr    = String(v.toString()).trimmed();
        return true;
    }
    
    bool        less(const T& a, const T& b) const override
    {
        return is_less( m_cs ? compare(a.*m_ptr,b.*m_ptr) : compare_igCase(a.*m_ptr,b.*m_ptr));
    }
    
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Enum member variable column
template <typename T, typename E>
struct SCMEnumMV : public StdColumn<T> {
    typedef E (T::*Ptr);
    Ptr         m_ptr;
    
    SCMEnumMV(const String& k, Ptr p) : StdColumn<T>(k), m_ptr(p)
    {
        StdColumn<T>::m_sortable  = true;
        StdColumn<T>::m_delegate.reset(new SDMSimple<T, EnumDelegate<E>>);
    }
    
    ~SCMEnumMV(){}
    
    QVariant    display(const T&d) const override
    {
        return QVariant::fromValue(d.*m_ptr);
    }
    
    bool        set(T& d, const QVariant& v) const override
    {
        d.*m_ptr    = v.value<E>();
        return true;
    }
    
    bool        less(const T& a, const T& b) const override
    {
        return a.*m_ptr < b.*m_ptr;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Enum-Map (one of) member variable column

template <typename T, typename K, typename V>
struct  SCMEnumMapMV : public StdColumn<T> {
    typedef EnumMap<K,V>    (T::*Ptr);
    Ptr             m_ptr;
    K               m_ekey;
    
    SCMEnumMapMV(const String&k, K e, Ptr p) : StdColumn<T>(k), m_ptr(p), m_ekey(e) 
    {
        StdColumn<T>::m_sortable  = true;
        StdColumn<T>::m_delegate.reset(new SDMSimple<T, EnumDelegate<V>>);
    }
    ~SCMEnumMapMV(){}
    
    QVariant    display(const T&d) const override
    {
        return QVariant::fromValue((d.*m_ptr)[m_ekey]);
    }

    bool        set(T& d, const QVariant& v) const override
    {
        (d.*m_ptr)[m_ekey]    = v.value<V>();
        return true;
    }
    
    bool        less(const T& a, const T& b) const override
    {
        return (a.*m_ptr)[m_ekey] < (b.*m_ptr)[m_ekey];
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! custom RO column

template <typename T, typename Getter>
struct SCMCustomRO : public StdColumn<T> {
    using Value  = std::decay_t<std::invoke_result_t<Getter,T>>;
    
    Getter      m_get;
    
    SCMCustomRO(const String&k, Getter g) : StdColumn<T>(k), m_get(g) 
    {
        StdColumn<T>::m_sortable  = yq::has_less_v<Value>;
    }
    
    ~SCMCustomRO(){}
    
    QVariant    display(const T& d) const override
    {
        if constexpr(std::is_same_v<Value,QVariant>){
            return m_get(d);
        } else
            return QVariant::fromValue(m_get(d));
    }
    
    bool        less(const T&a, const T&b) const override
    {
        if constexpr (std::is_same_v<Value, QString>){
            return compare_igCase(m_get(a), m_get(b));
        } else if constexpr (std::is_same_v<Value, String>){
            return compare_igCase(m_get(a), m_get(b));
        } else if constexpr (yq::has_less_v<Value>){
            return m_get(a) < m_get(b);
        } else
            return false;
    }
};

template <typename T, typename Getter, typename Less>
struct SCMCustomROL : public StdColumn<T> {
    using Value  = std::decay_t<std::invoke_result_t<Getter,T>>;
    Getter      m_get;
    Less        m_less;
    
    SCMCustomROL(const String&k, Getter g, Less l) : StdColumn<T>(k), m_get(g), m_less(l)
    {
        StdColumn<T>::m_sortable  = true;
    }
    
    ~SCMCustomROL(){}
    
    QVariant    display(const T& d) const override
    {
        if constexpr(std::is_same_v<Value,QVariant>){
            return m_get(d);
        } else
            return QVariant::fromValue(m_get(d));
    }
    
    bool        less(const T&a, const T& b) const override
    {
        return m_less(a,b);
    }
};
    
template <typename T, typename Getter, typename Setter>
struct SCMCustomRW : public StdColumn<T> {
    using Value  = std::decay_t<std::invoke_result_t<Getter,T>>;
    Getter      m_get;
    Setter      m_set;
    
    SCMCustomRW(const String&k, Getter g, Setter s) : StdColumn<T>(k), m_get(g), m_set(s)
    {
        StdColumn<T>::m_settable  = true;
        StdColumn<T>::m_sortable  = yq::has_less_v<Value>;
    }
    
    ~SCMCustomRW(){}
    
    QVariant    display(const T& d) const override
    {
        if constexpr(std::is_same_v<Value,QVariant>){
            return m_get(d);
        } else
            return QVariant::fromValue(m_get(d));
    }
    
    bool        set(T& d, const QVariant& v) const override
    {
        // TODO ... use template specialization to make this smarter....
        return m_set(d,v.value<Value>());
    }

    bool        less(const T&a, const T&b) const override
    {
        if constexpr (std::is_same_v<Value, QString>){
            return compare_igCase(m_get(a), m_get(b));
        } else if constexpr (std::is_same_v<Value, String>){
            return compare_igCase(m_get(a), m_get(b));
        } else if constexpr (yq::has_less_v<Value>){
            return m_get(a) < m_get(b);
        } else
            return false;
    }
};

template <typename T, typename Getter, typename Setter, typename Less>
struct SCMCustomRWL : public StdColumn<T> {
    using Value  = std::decay_t<std::invoke_result_t<Getter,T>>;
    Getter      m_get;
    Setter      m_set;
    Less        m_less;
    
    
    SCMCustomRWL(const String& k, Getter g, Setter s, Less l) : StdColumn<T>(k), m_get(g), m_set(s), m_less(l)
    {
        StdColumn<T>::m_settable  = true;
        StdColumn<T>::m_sortable  = true;
    }
    
    ~SCMCustomRWL(){}
    
    QVariant    display(const T& d) const override
    {
        if constexpr(std::is_same_v<Value,QVariant>){
            return m_get(d);
        } else
            return QVariant::fromValue(m_get(d));
    }
    
    bool        set(T& d, const QVariant& v) const override
    {
        // TODO ... use template specialization to make this smarter....
        return m_set(d,v.value<Value>());
    }

    bool        less(const T&a, const T& b) const override
    {
        return m_less(a,b);
    }
};

