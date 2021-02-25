////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StdFile.hpp"

class FolderFile : public StdFile {
public:

    static constexpr Seq         zIcon      = { "icon" };
    static constexpr Seq         zName      = { "%", "%name", "name" };
    static constexpr const char *szFile     = ".folder";

    String      icon() const;
    void        icon(const String&);

    String      name() const;
    void        name(const String&);

private:
};
