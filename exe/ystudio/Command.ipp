////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"

struct Command::Repo {
    std::vector<const Command*>     all;
    
    Repo()
    {
        all.push_back(nullptr);
    }
    
    size_t count() const { return all.size(); }
};

Command::Repo& Command::repo()
{
    static Repo s_repo;
    return s_repo;
}

std::vector<const Command*>  Command::all()
{
    return repo().all;
}

const Command*               Command::get(uint64_t i)
{
    Repo& _r    = repo();
    if(i<_r.all.size())
        return _r.all[i];
    return nullptr;
}


////////////////////////////////////////////////////////////////////////////////

Command::Command(const QString&kLabel) : m_id(repo().count()), m_label(kLabel)
{
    repo().all.push_back(this);
}

////////////////////////////////////////////////////////////////////////////////

namespace {
    struct CommandDW : public Command {
        using FN    = std::function<void(DreamMW*)>;
        FN          m_function;
    
        CommandDW(const QString& lab, FN&& fn) : Command(lab), m_function(std::move(fn))
        {
            m_flavor    = ArgFlavor::Main;
            m_types     = ~ IdTypes();
        }

        void        invoke(DreamMW* dw, Id) const
        {
            m_function(dw);
        }
        
        bool        accept(Id i) const 
        {
            return m_types[i.type()];
        }
        
    };
}

Command::Writer Command::reg( const QString&label, std::function<void(DreamMW*)>&& fn)
{
    if(!fn)
        return Writer();
    return Writer(new CommandDW(label, std::move(fn)));
}

namespace {
    struct CommandId : public Command {
        using FN    = std::function<void(DreamMW*, Id)>;
        FN m_function;
        CommandId(const QString& lab, FN&& fn) : Command(lab), m_function(std::move(fn))
        {
            m_flavor    = ArgFlavor::Id;
            m_types     = ~IdTypes();
        }
        void        invoke(DreamMW* dw, Id i) const
        {
            m_function(dw, i);
        }
        
        bool        accept(Id i) const 
        {
            return m_types[i.type()];
        }
    };
}

Command::Writer Command::reg( const QString&label, std::function<void(DreamMW*,Id)>&& fn)
{
    if(!fn)
        return Writer();
    return Writer(new CommandId(label, std::move(fn)));
}

namespace {
    struct CommandIdF : public Command {
        using FN    = std::function<void(DreamMW*, Id)>;
        using FNF   = std::function<bool(Id)>;
        FN          m_function;
        FNF         m_filter;
        CommandIdF(const QString& lab, FN&& fn, FNF&& fnf) : 
            Command(lab), m_function(std::move(fn)), m_filter(std::move(fnf))
        {
            m_flavor    = ArgFlavor::Id;
            m_types     = ~IdTypes();
        }
        void        invoke(DreamMW* dw, Id i) const
        {
            m_function(dw, i);
        }
        
        bool        accept(Id i) const 
        {
            if(! m_types[i.type()])
                return false;
            return m_filter(i);
        }
    };
}

Command::Writer     Command::reg( const QString&label, std::function<void(DreamMW*,Id)>&&fn, std::function<bool(Id)>&& fnF)
{
    if(!fn)
        return Writer();
    if(!fnF)
        return Writer();
    return Writer(new CommandIdF(label, std::move(fn), std::move(fnF)));
}

