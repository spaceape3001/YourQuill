////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string_view>

class QByteArray;
class QString;

namespace yq {
    class String;
    class StringView;

    /*! \brief Useful parameter for case-insensitive string keys in sets and maps
    */
    struct IgCase {
        bool    operator()(const std::string_view&, const std::string_view&) const;
        bool    operator()(const QString&, const QString&) const;
        bool    operator()(const QByteArray&, const QByteArray&) const;
    };

    /*! \brief Useful parameter for case-insensitive string keys in sets and maps
    */
    struct RevIgCase {
        bool    operator()(const std::string_view&, const std::string_view&) const;
        bool    operator()(const QString&, const QString&) const;
        bool    operator()(const QByteArray&, const QByteArray&) const;
    };
}
