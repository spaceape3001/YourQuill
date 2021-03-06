////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <db/SizeDesc.hpp>
#include <util/Ref.hpp>
#include <QByteArray>
#include <atomic>

class Root;

struct ID {
    uint64_t        id;
    int64_t         created;
    uint64_t        magic[14];
    
    void            make();
    void            newId();
    QByteArray      net() const;;
    bool            operator==(const ID&b) const;
};

//YQ_ENUM( IconSize, , Small, Medium, Large )

struct Session : public ID, public RefCount {
    QByteArray                  user;
    bool                        loggedIn;
    int64_t                     accessed;
    std::atomic<const Root*>    defRoot;
    std::atomic<bool>           autoEdit;
    SizeDesc                    iconSize;
    bool                        inspectSubmit;
    unsigned int                columns;
    
    Session();  // non-trivial
};

//!  Makes/create a session 
Ref<Session>      sessionFor(const QByteArray& decl);

