////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

class QByteArray;
class QString;

namespace yq {
    class String;
    class StringView;

    /*! \brief Useful parameter for case-insensitive string keys in sets and maps
    */
    struct IgCase {
        bool    operator()(const String&, const String&) const;
        bool    operator()(const StringView&, const StringView&) const;
        bool    operator()(const QString&, const QString&) const;
        bool    operator()(const QByteArray&, const QByteArray&) const;
    };

    /*! \brief Useful parameter for case-insensitive string keys in sets and maps
    */
    struct RevIgCase {
        bool    operator()(const String&, const String&) const;
        bool    operator()(const StringView&, const StringView&) const;
        bool    operator()(const QString&, const QString&) const;
        bool    operator()(const QByteArray&, const QByteArray&) const;
    };
}
