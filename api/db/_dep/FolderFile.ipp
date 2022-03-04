////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    String      FolderFile::icon() const
    {
        return value(zIcon);
    }

    void        FolderFile::icon(const String&s)
    {
        set(zIcon, s, true);
    }

    String      FolderFile::name() const
    {
        return value(zName);
    }

    void        FolderFile::name(const String&s)
    {
        set(zName, s, true);
    }


}
