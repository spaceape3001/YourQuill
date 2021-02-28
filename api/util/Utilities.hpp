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

QByteArray      make_png(const QImage&);

QString         sanitize_path(const QString&);
QStringList     sanitize_path(const QStringList&);

QString         strip_extension(const QString&);

QString         utf8(const QByteArray&);
QByteArray      utf8(const QString&);

std::ostream& operator<<(std::ostream&, const QString&);

StringSet       qt(const QStringSet&);
QStringSet      qt(const StringSet&);

Vector<String>  qt(const Vector<QString>&);
Vector<QString> qt(const Vector<String>&);
