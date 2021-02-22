////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Session.hpp"
#include "yq/util/Utilities.hpp"

#include <sys/random.h>
#include <tbb/spin_rw_mutex.h>
#include <QDateTime>

void    ID::make()
{
    for(size_t  sz  = 0; sz < sizeof(magic); )
        sz += getrandom( sz + (char*) magic, sizeof(magic)-sz, 0);
    created = QDateTime::currentSecsSinceEpoch();
}
    
void    ID::newId()
{
    do {
        id = randU64();
    } while(!id);
}

QByteArray      ID::net() const
{
    return QByteArray((const char*) this, sizeof(*this));
}
    
bool            ID::operator==(const ID&b) const
{
    return !memcmp(this, &b, sizeof(*this));
}

Session::Session() : ID{}, loggedIn(false), accessed(0)
{
    defRoot         = nullptr;
    autoEdit        = false;
    inspectSubmit   = false;
    columns         = 0;
    newId();
}


Ref<Session>      sessionFor(const QByteArray& decl)
{
    static tbb::spin_rw_mutex             s_mutex;
    static Hash<uint64_t,Ref<Session>>    s_sessions;
    
    Ref<Session>      ret;
    if(!decl.isEmpty() && (decl.size() >= (int) sizeof(ID))){
        const ID*   h   = (const ID*) decl.constData();
        {
            tbb::spin_rw_mutex::scoped_lock _lock(s_mutex, false);
            ret = s_sessions.get(h -> id);
        }
        if(ret.valid() && (*ret == *h))
            return ret;
    }
    
    ret = new Session;
    {
        tbb::spin_rw_mutex::scoped_lock _lock(s_mutex, true);
        while( s_sessions.has(ret -> id))
            ret -> newId();
        s_sessions[ret -> id]  = ret;
    }
    return ret;
}
