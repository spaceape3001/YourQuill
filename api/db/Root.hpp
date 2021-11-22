////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootFwd.hpp"
#include <db/bit/Policy.hpp>
#include <db/enum/Access.hpp>
#include <db/enum/Vcs.hpp>

#include <util/EnumMap.hpp>
#include <util/Flag.hpp>

#include <filesystem>

namespace wksp { struct Init; }

/*! \brief Represents a "root" directory for the workspace, all keys are relative to one of these.
*/
struct Root {
    friend struct wksp::Init;
    using fspath = std::filesystem::path;
    
    EnumMap<DataRole, Access>   access;
    String                      color;
    uint64_t                    depth           = 0;    // used by the workspace during load to shuffle into order
    String                      icon;               // file
    uint64_t                    id              = 0;
    bool                        is_template     = false;
    String                      key;
    String                      name;
    const std::filesystem::path path;
    Flag<Vcs>                   vcs;
    

    static const Root*  by_key(const std::string_view&);

    bool                has_git() const;
    bool                has_subversion() const;

    //  version control
    void                move(const std::filesystem::path&, const std::filesystem::path&);
    void                remove(const std::filesystem::path&);
    void                add(const std::filesystem::path&);
    bool                commit();
    bool                update();


    bool                exists(const char*) const;
    bool                exists(const QString&) const;
    bool                exists(const QByteArray&) const;
    bool                exists(const std::string&) const;
    bool                exists(const std::string_view&) const;
    bool                exists(const std::filesystem::path&) const;
    
    bool                make_path(const char*) const;
    bool                make_path(const QByteArray&) const;
    bool                make_path(const QString&) const;
    bool                make_path(const std::string&) const;
    bool                make_path(const std::string_view&) const;
    bool                make_path(const std::filesystem::path&) const;

    //  From the quill-doc (relative)
    //const String&       def_icon_file() const { return m_icon; }
    
    bool                is_readable(DataRole) const;
    bool                is_writable(DataRole) const;
    bool                is_write_first(DataRole) const;

    std::filesystem::path   resolve(const char*     ) const;
    std::filesystem::path   resolve(const QByteArray& ) const;
    std::filesystem::path   resolve(const QString& ) const;
    std::filesystem::path   resolve(const std::string& ) const;
    std::filesystem::path   resolve(const std::string_view& ) const;
    std::filesystem::path   resolve(const std::filesystem::path&) const;
    
    
    Access              policy(DataRole) const;
    
private:

    Root(const std::filesystem::path&);
    Root(const std::filesystem::path&, PolicyMap);


    Root(const Root&) = delete;
    Root(Root&&) = delete;
    
    ~Root();
    
    Root&    operator=(const Root&) = delete;
    Root&    operator=(Root&&) = delete;
    
};




