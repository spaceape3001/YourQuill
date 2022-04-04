#pragma once

#include <yq/collection/Deque.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/file/DirWatcher.hpp>

namespace yq {
    class Notifier;
    class Root;
}

using namespace yq;

struct RootPath {
    const yq::Root*             root    = nullptr;
    std::filesystem::path       path;       //!< Relative path if root present, absolute otherwise
};

#if 0
class YScanner : public yq::DirWatcher {
public:


    YScanner();
    ~YScanner();
    
    
    void    prime();
    void    do_work();
    
    bool    is_bad() const { return m_bad; }

private:

    enum State {
        Init        = 0,
        Startup,
        Scanning
    };
    

    State                   m_state = Init;

    void                check(const std::filesystem::path&);
    //const yq::Root*     root(const std::filesystem::path&) const;


    
    struct D;
    struct F;
    struct W;
    struct T;

    using WatchList = yq::Vector<const yq::Notifier*> ;

    yq::Deque<D>            m_scan;
    yq::Vector<W>           m_watch;
    yq::StringSet           m_ignore;
    double                  m_time          = 0.;
    uint64_t                m_nFragments    = 0;
    unsigned                m_nIter         = 0;
    bool                    m_primed        = false;
    bool                    m_first         = true;
    bool                    m_startup       = false;
    bool                    m_bad           = false;
    //yq::EnumMap<Change, T>  m_notify;
    
    
    
    
    
    //D                   next();
    void     scan();
    void     checkWatch();
};
#endif

