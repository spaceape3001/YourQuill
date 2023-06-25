////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ystudio.hpp"

#include <QString>
#include <QIcon>
#include <QKeySequence>

class DreamMW;

class Command {
public:
    class Writer;
    
    virtual void        invoke(DreamMW*, Id) const = 0;
    virtual bool        accept(Id) const { return true; }

    
    ArgFlavor           flavor() const { return m_flavor; }

    uint64_t            id() const { return m_id; }
    const QString&      menu() const { return m_menu; }
    const QString&      label() const { return m_label; }
    const QIcon&        icon() const { return m_icon; }
    const QKeySequence& shortcut() const { return m_shortcut; }
    const QString&      toolTip() const { return m_toolTip; }
    int                 gravity() const { return m_gravity; }
    
    //  This command "opens" the specified type, if accepted
    IdTypeId            opener() const { return m_opens; }
    
    static std::vector<const Command*>  all();
    static const Command*               get(uint64_t);
    static Writer                       reg( const QString&, std::function<void(DreamMW*)>&& );
    static Writer                       reg( const QString&, std::function<void(DreamMW*,Id)>&& );
    static Writer                       reg( const QString&, std::function<void(DreamMW*,Id)>&&, std::function<bool(Id)>&& );
    
    template <yq::mithril::IdType S>
    static Writer                       reg( const QString& l, std::function<void(DreamMW*,S)>&& fn);

protected:
    const uint64_t  m_id;
    ArgFlavor       m_flavor    = ArgFlavor::Dead;
    IdTypes         m_types     = ~IdTypes{};
    QString         m_menu;
    QString         m_label;
    QString         m_toolTip;
    QIcon           m_icon;
    QKeySequence    m_shortcut;
    int             m_gravity   = 0;    // used for ordering items
    IdTypeId        m_opens     = 0;

    Command(const QString&);
    
    Command(const Command&) = delete;
    Command(Command&&) = delete;
    Command& operator=(const Command&) = delete;
    Command& operator=(Command&&) = delete;
    
    struct Repo;
    static Repo& repo();
};

class Command::Writer {
public:

    Writer() {}

    Writer&     menu(const QString& s)
    {
        if(m_cmd)
            m_cmd -> m_menu = s;
        return *this;
    }

    Writer&     toolTip(const QString& s)
    {
        if(m_cmd)
            m_cmd -> m_toolTip = s;
        return *this;
    }

    Writer&     tooltip(const QString& s)
    {
        if(m_cmd)
            m_cmd -> m_toolTip = s;
        return *this;
    }
    
    Writer& shortcut(const QKeySequence& ks)
    {
        if(m_cmd)
            m_cmd -> m_shortcut = ks;
        return *this;
    }
    
    Writer& icon(const QIcon& ico)
    {
        if(m_cmd)
            m_cmd -> m_icon = ico;
        return *this;
    }
    
    Writer& gravity(int g)
    {
        if(m_cmd)
            m_cmd -> m_gravity = g;
        return *this;
    }
    

    Writer& types(IdTypes t)
    {
        if(m_cmd)
            m_cmd -> m_types    = t;
        return *this;
    }
    
    template <yq::mithril::IdType S>
    Writer& opener()
    {
        if(m_cmd){
            m_cmd -> m_opens = S::ID;
            m_cmd -> m_types = IdTypes(S::ID);
        }
        return *this;
    }

private:
    friend class  Command;
    Writer(Command* c) : m_cmd(c) {}
    Command*    m_cmd  = nullptr;
};

template <yq::mithril::IdType S>
Command::Writer Command::reg( const QString& label, std::function<void(DreamMW*, S)>&& fn)
{
    Writer  w   = reg(label, [fn](DreamMW* mw, Id i){
        if(i.type() == S::ID)
            fn(mw, S(i.id()));
    });
    w.types(S::ID);
    return w;
}

