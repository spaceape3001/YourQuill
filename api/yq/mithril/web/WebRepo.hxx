////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <tbb/spin_rw_mutex.h>
#include <vector>
#include <yq/container/EnumMap.hpp>
#include <yq/container/Map.hpp>
#include <yq/core/Ref.hpp>
#include <yq/net/HttpOp.hpp>
#include <yq/text/IgCase.hpp>

    // Conditional read lock
#define LOCK                                                        \
    const auto& _r = webRepo();                                     \
    tbb::spin_rw_mutex::scoped_lock     _lock;                      \
    if(_r.openReg)                                                  \
        _lock.acquire(_r.mutex, false);

    // Unconditional read lock
#define LLOCK                                                       \
    const auto& _r = webRepo();                                     \
    tbb::spin_rw_mutex::scoped_lock     _lock(_r.mutex, false);     \

    // Unconditional write lock
#define WLOCK                                                       \
    auto& _r = webRepo();                                           \
    tbb::spin_rw_mutex::scoped_lock     _lock(_r.mutex, true);


namespace yq::mithril {

    class WebPage;
    class WebVariable;

    using WebPageMap    = EnumMap<HttpOp, Map<std::string_view, const WebPage*, IgCase>>;
    using WebVarMap     = Map<std::string_view, const WebVariable*, IgCase>;

    class Template;
    
    struct WebRepo {
        std::vector<const WebPage*>     allPages;
        std::vector<const WebVariable*> allVars;
        WebPageMap                      dirs;
        WebPageMap                      exts;
        WebPageMap                      globs;
        WebPageMap                      pages;
        WebVarMap                       variables;
        Ref<Template>                   htmlTemplate;
        mutable tbb::spin_rw_mutex      mutex;
        volatile bool                   openReg    = true;
        
            // This *IS* case sensitive!
        //Map<std::string_view, Ref<WebSession>>  sessions;
        
        WebRepo();
    };

    WebRepo& webRepo();
}
