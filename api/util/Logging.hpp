////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DebugRel.hpp"
#include "Enum.hpp"
#include <log4cpp/Category.hh>
#include <log4cpp/CategoryStream.hh>
#include <log4cpp/Priority.hh>
#include <filesystem>
#include <optional>

YQ_ENUM(LogPriority, , 
    NotSet,
    All,
    Debug,
    Info,
    Notice,
    Warning,
    Error,
    Critical,
    Alert,
    Fatal,
    Emergency
)

class QDate;
class QDateTime;
class QDir;
class QSqlError;
class QUrl;

class String;

// suggestions (not required)
static constexpr const auto  kLogDef_Console = YQ_DBGREL(LogPriority::Debug, LogPriority::Warning);
static constexpr const auto  kLogDef_File    = YQ_DBGREL(LogPriority::Deubg, LogPriority::Info);

log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&, const QByteArray&);
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&, const QDate&);
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&, const QDateTime&);
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&, const QDir&);
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&, const QSqlError&);
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&, const QString&);
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&, const QUrl&);

template <typename E>
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&str, EnumImpl<E> v)
{
    return str << v.key();
}

template <typename T>
log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream& str, const std::optional<T>&v)
{
    if(v)
        str << *v;
    else
        str << "(unset)";
    return str;
}


log4cpp::Category&          log_category(const char* z=nullptr);

//int							yLogPriority(LogPriority);

void                        log_to_std_error(LogPriority p = LogPriority());
void                        log_to_std_output(LogPriority p = LogPriority());
void                        log_to_file(const std::filesystem::path&, LogPriority p = LogPriority());

//void                        log_freeze();

LogPriority                 log_std_output_priority();
LogPriority                 log_std_error_priority();
bool                        is_log_std_output_enabled();
bool                        is_log_std_error_enabled();


#define yAlert(...)         log_category(__VA_ARGS__).alertStream()
#define yCritical(...)      log_category(__VA_ARGS__).critStream()
#define yDebug(...)         log_category(__VA_ARGS__).debugStream()
#define yError(...)         log_category(__VA_ARGS__).errorStream()
#define yEmergency(...)     log_category(__VA_ARGS__).emergStream()
#define yFatal(...)         log_category(__VA_ARGS__).fatalStream()
#define yInfo(...)          log_category(__VA_ARGS__).infoStream()
#define yNotice(...)        log_category(__VA_ARGS__).noticeStream()
#define yWarning(...)       log_category(__VA_ARGS__).warnStream()

