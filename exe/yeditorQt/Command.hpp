////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ystudio.hpp"
#include "Action.hpp"

namespace yq::mithril {
    class AtomSpec;
    class FileSpec;
}


class DreamMW;

class Command {
public:

    using DwInvokeFN    = std::function<void(DreamMW*)>;
    using DwIdInvokeFN  = std::function<void(DreamMW*,Id)>;
    using IdInvokeFN    = std::function<void(Id)>;
    using FilterFN      = std::function<bool(Id)>;

    enum class Type {
        //! Generic non-command
        None    = 0,
        View,
        Edit,
        Delete,
        Properties,
        
        COUNT
    };

    class Writer;
    
    virtual void        invoke(DreamMW*, Id) const = 0;
    virtual bool        accept(Id) const { return true; }

    
    ArgFlavor           flavor() const { return m_flavor; }
    Type                type() const { return m_type; }

    const ActionInfo&   action() const { return m_action; }

    uint64_t            id() const { return m_id; }
    const QString&      menu() const { return m_menu; }
    const QString&      label() const { return m_action.label; }
    const QIcon&        icon() const { return m_action.icon; }
    const QKeySequence& shortcut() const { return m_action.shortcut; }
    const QString&      toolTip() const { return m_action.toolTip; }
    int                 gravity() const { return m_gravity; }
    
    //  This command "opens" the specified type, if accepted
    IdTypeId            opener() const { return m_opens; }
    IdTypes             idTypes() const { return m_idTypes; }
    
    static std::vector<const Command*>  all();
    static const Command*               get(uint64_t);
    static Writer                       reg( const QString&, DwInvokeFN&& );
    static Writer                       reg( const QString&, DwIdInvokeFN&& );
    static Writer                       reg( const QString&, FilterFN&&, DwIdInvokeFN&& );
    
    /*! \brief Registers an atom spec for the filter
    */
    static Writer                       reg( const QString&, yq::mithril::AtomSpec&&, DwIdInvokeFN&& );

    /*! \brief Registers a file spec for the filter
    */
    static Writer                       reg( const QString&, yq::mithril::FileSpec&&, DwIdInvokeFN&& );
    
    template <yq::mithril::IdType S>
    static Writer                       reg( const QString& l, std::function<void(DreamMW*,S)>&& fn);
    
    static uint64_t     count();

protected:
    const uint64_t  m_id;
    ArgFlavor       m_flavor    = ArgFlavor::Dead;
    IdTypes         m_idTypes   = ~IdTypes{};
    QString         m_menu;
    ActionInfo      m_action;
    int             m_gravity   = 0;    // used for ordering items
    IdTypeId        m_opens     = 0;
    Type            m_type      = Type::None;

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
            m_cmd -> m_action.toolTip = s;
        return *this;
    }

    Writer&     tooltip(const QString& s)
    {
        if(m_cmd)
            m_cmd -> m_action.toolTip = s;
        return *this;
    }
    
    Writer& shortcut(const QKeySequence& ks)
    {
        if(m_cmd)
            m_cmd -> m_action.shortcut = ks;
        return *this;
    }
    
    Writer& icon(const QIcon& ico)
    {
        if(m_cmd)
            m_cmd -> m_action.icon = ico;
        return *this;
    }
    
    Writer& gravity(int g)
    {
        if(m_cmd)
            m_cmd -> m_gravity = g;
        return *this;
    }
    
    Writer& type(IdTypeId t)
    {
        if(m_cmd)
            m_cmd -> m_idTypes   |= t;
        return *this;
    }

    template <yq::mithril::IdType S>
    Writer& type()
    {
        if(m_cmd)
            m_cmd -> m_idTypes   |= S::ID;
        return *this;
    }


    Writer& types(IdTypes t)
    {
        if(m_cmd)
            m_cmd -> m_idTypes    = t;
        return *this;
    }
    
    //! Marks this as a viewer for specified type (assuming it's accepted)
    template <yq::mithril::IdType S>
    Writer& viewer()
    {
        if(m_cmd){
            m_cmd -> m_opens    = S::ID;
            m_cmd -> m_idTypes  = IdTypes(S::ID);
            m_cmd -> m_type     = Type::View;
        }
        return *this;
    }

    //! Marks this as a editor for specified type (assuming it's accepted)
    template <yq::mithril::IdType S>
    Writer& editor()
    {
        if(m_cmd){
            m_cmd -> m_opens    = S::ID;
            m_cmd -> m_idTypes  = IdTypes(S::ID);
            m_cmd -> m_type     = Type::Edit;
        }
        return *this;
    }

    template <yq::mithril::IdType S>
    Writer& deletes()
    {
        if(m_cmd){
            m_cmd -> m_opens    = S::ID;
            m_cmd -> m_idTypes  = IdTypes(S::ID);
            m_cmd -> m_type     = Type::Delete;
        }
        return *this;
    }
    
    template <yq::mithril::IdType S>
    Writer& properties()
    {
        if(m_cmd){
            m_cmd -> m_opens    = S::ID;
            m_cmd -> m_idTypes  = IdTypes(S::ID);
            m_cmd -> m_type     = Type::Properties;
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

