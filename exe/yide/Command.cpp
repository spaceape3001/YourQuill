////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include <yq/mithril/atom/AtomSpec.hpp>
#include <yq/mithril/file/FileSpec.hpp>

using namespace yq;
using namespace yq::mithril;

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

uint64_t     Command::count()
{
    return repo().all.size();
}



////////////////////////////////////////////////////////////////////////////////

Command::Command(const QString&kLabel) : m_id(repo().count())
{
    m_action.label  = kLabel;
    repo().all.push_back(this);
}

////////////////////////////////////////////////////////////////////////////////

namespace {
    struct CommandDW : public Command {
        DwInvokeFN      m_function;
    
        CommandDW(const QString& lab, DwInvokeFN&& fn) : Command(lab), m_function(std::move(fn))
        {
            m_flavor    = ArgFlavor::Main;
        }

        void        invoke(DreamMW* dw, Id) const
        {
            m_function(dw);
        }
        
        bool        accept(Id i) const 
        {
            return m_idTypes[i.type()];
        }
        
    };
}

Command::Writer Command::reg( const QString&label, DwInvokeFN&& fn)
{
    if(!fn)
        return Writer();
    return Writer(new CommandDW(label, std::move(fn)));
}

namespace {
    struct CommandId : public Command {
        DwIdInvokeFN    m_function;
        CommandId(const QString& lab, DwIdInvokeFN&& fn) : Command(lab), m_function(std::move(fn))
        {
            m_flavor    = ArgFlavor::Id;
        }
        void        invoke(DreamMW* dw, Id i) const
        {
            m_function(dw, i);
        }
        
        bool        accept(Id i) const 
        {
            return m_idTypes[i.type()];
        }
    };
}

Command::Writer Command::reg( const QString&label, DwIdInvokeFN&& fn)
{
    if(!fn)
        return Writer();
    return Writer(new CommandId(label, std::move(fn)));
}

namespace {
    struct CommandIdF : public Command {
        FilterFN         m_filter;
        DwIdInvokeFN     m_function;
        
        CommandIdF(const QString& lab, FilterFN&& fil, DwIdInvokeFN&& fn) : 
            Command(lab), m_filter(std::move(fil)), m_function(std::move(fn))
        {
            m_flavor    = ArgFlavor::Id;
        }
        
        void        invoke(DreamMW* dw, Id i) const
        {
            m_function(dw, i);
        }
        
        bool        accept(Id i) const 
        {
            if(! m_idTypes[i.type()])
                return false;
            return m_filter(i);
        }
    };
}

Command::Writer     Command::reg( const QString&label, FilterFN&& fnF, DwIdInvokeFN&&fn)
{
    if(!fn)
        return Writer();
    if(!fnF)
        return Writer();
    return Writer(new CommandIdF(label, std::move(fnF), std::move(fn)));
}

namespace {
    struct CommandAtomSpec : public Command {
        AtomSpec        m_filter;
        DwIdInvokeFN    m_function;
        
        CommandAtomSpec(const QString&label, const AtomSpec&& spec, DwIdInvokeFN&& fn) : 
            Command(label), m_filter(std::move(spec)), m_function(std::move(fn))
        {
            m_flavor    = ArgFlavor::Id;
        }
        
        void        invoke(DreamMW* dw, Id i) const
        {
            m_function(dw, i);
        }
        
        bool        accept(Id i) const 
        {
            if(! m_idTypes[i.type()])
                return false;
            return m_filter.match(i);
        }
    };
}

Command::Writer     Command::reg( const QString&label, AtomSpec&&spec, DwIdInvokeFN&& fn)
{
    if(!fn)
        return Writer();
    return Writer(new CommandAtomSpec(label, std::move(spec), std::move(fn)));
}

namespace {
    struct CommandFileSpec : public Command {
        FileSpec        m_filter;
        DwIdInvokeFN    m_function;
        
        CommandFileSpec(const QString&label, const FileSpec&& spec, DwIdInvokeFN&& fn) : 
            Command(label), m_filter(std::move(spec)), m_function(std::move(fn))
        {
            m_flavor    = ArgFlavor::Id;
        }
        
        void        invoke(DreamMW* dw, Id i) const
        {
            m_function(dw, i);
        }
        
        bool        accept(Id i) const 
        {
            if(!m_idTypes[i.type()])
                return false;
            return m_filter.match(i);
        }
    };
}

Command::Writer     Command::reg( const QString&label, FileSpec&&spec, DwIdInvokeFN&& fn)
{
    if(!fn)
        return Writer();
    return Writer(new CommandFileSpec(label, std::move(spec), std::move(fn)));
}
