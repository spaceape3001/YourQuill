////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TypeFwd.hpp"
#include <cstdint>
#include <iosfwd>

class QChar;
class QRect;
class QIcon;
class QImage;
class QString;
class QStringList;


uint64_t        randU64();

/*! \brief Returns desktop parameters.

    \note, there can be GAPS due to relative screen geometry
*/
QRect           desktop_screen_box();

/*!
    Gets an icon from a filename.  Any %1 notation will be replaced by 
    common icon sizes and tested.
    
    \note Qt restriction requires this to run on the MAIN THREAD.  Any
    other thread will simply return an empty icon
*/
QIcon           fetch_icon(const QString&);




bool            is_main_thread();
bool            isMainThread();

bool            is_in(const String& key, const String& b);
bool            is_in(const String& key, const char* b);
bool            is_in(const char* key, const String& b);
bool            is_in(const Vector<String>& keys, const String& b);
bool            is_in(const std::initializer_list<String>& keys, const String& b);
bool            is_in(const std::initializer_list<const char*>& keys, const String& b);


QString         join(const Set<uint16_t>& values, const QString& sep);

QString         join(const QStringSet&, QChar);
QString         join(const QStringSet&, const QString&);

QByteArray      join(const Vector<QByteArray>&, char);
QByteArray      join(const Vector<QByteArray>&, const QByteArray&);

QString         join(const Vector<QString>&, char);
QString         join(const Vector<QString>&, const QString&);

String          join(const Vector<String>&, char);
String          join(const Vector<String>&, const String&);
String          join(const StringSet&, const String&);

String          join_string(const Set<uint16_t>& values, const String& sep);
QString         join_qstring(const Set<uint16_t>& values, const QString& sep);

QByteArray      make_png(const QImage&);

QString         sanitize_path(const QString&);
QStringList     sanitize_path(const QStringList&);

QString         strip_extension(const QString&);

QString         utf8(const QByteArray&);
QByteArray      utf8(const QString&);

std::ostream& operator<<(std::ostream&, const QString&);

StringSet       qt(const QStringSet&);
QStringSet      qt(const StringSet&);
QStringList     qt_list(const QStringSet&);
QStringList     qt_list(const StringSet&);

Vector<String>  qt(const Vector<QString>&);
Vector<QString> qt(const Vector<String>&);

QList<QVariant>    qvar_list(const StringSet& slist);
QList<QVariant>    qvar_list(const QStringSet& slist);

QIcon           theme_icon(const QString& icon);

/*! \brief Gets an icon from theme, or the filename as a fallback 

    This is *BETTER* than QIcon's fromTheme because we won't do an unneccessary load of the fallback icon
*/
QIcon           theme_icon(const QString& icon, const QString& fallback);
