////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QSqlQuery>
#include <QVariant>
#include "Map.hpp"
#include <filesystem>


//  warning, do not allocate on the heap, pass it around as a QSqlQuery...it'll slice!
//  fortunately, there's little point, as qt does an internal pImpl pointer pattern
//  anyways.
struct SqlQuery : public QSqlQuery {

    struct AutoFinish;

    SqlQuery(QSqlDatabase, const QString& s=QString());
    SqlQuery(SqlQuery&&);
    ~SqlQuery();
    SqlQuery&   operator=(SqlQuery&&);
    
    AutoFinish  af();
    
    bool        batch(bool fNoisy=true);
    void        bind(int n, const QByteArray& val);
    void        bind(int n, const QVariant& val);
    void        bind(int n, const String&);
    void        bind(int n, const QString&);
    void        bind(int n, const std::filesystem::path&);
    void        bind(int n, uint64_t val);
    bool        exec(bool fNoisy=true);
    
    
    bool        prepare(const QString&);
    
    struct Attachment {
        virtual void            value(const QVariant&) = 0;
        virtual ~Attachment(){}
    };
    
    // warning, this class takes ownership of the pointer, deletes it
    void        attach(int, Attachment*);
    void        attach(int, QStringList&);
    void        attach(int, StringSet&);
    void        attach(int, Vector<QString>&);
   
    template <typename P>
    struct LambdaAttachment;
    
    template <typename P>
    void        attach(int, P&&);
   
    uint64_t    lastInsertIdU64() const;

    //  processes the results by pushing it into the "attachments"
    void        process();
   
   
    template <typename T>
    T           valueAs(int n) const
    {
        return value(n).value<T>();
    }

    template <typename T>
    T           valueAs(const QString&n) const
    {
        return value(n).value<T>();
    }
    
    
    bool        valueBool(int n) const;
    
    QByteArray  valueBytes(int n) const;
    QString     valueString(int n) const;
    std::filesystem::path   valuePath(int n) const;

    uint64_t    valueU64(int n) const;

    /*! \brief Shortcut for gettings a single-column result 
    */
    template <typename T>
    void        getAll(int col, T& val)
    {
        attach(col, val);
        process();
    }

private:
    SqlQuery(const SqlQuery&) = delete;
    SqlQuery&   operator=(const SqlQuery&) = delete;
    
    void        setAttach(int, Attachment*);
    Map<int, Attachment*>   m_attach;
};

template <typename P>
struct SqlQuery::LambdaAttachment : public Attachment {
    P       pred;
    LambdaAttachment(P&& _pred) : pred(std::move(_pred)) 
    {
    }
    
    void    value(const QVariant& v) override 
    {
        pred(v);
    }
};

template <typename P>
void SqlQuery::attach(int n, P&& pred)
{
    setAttach(n, new LambdaAttachment<P>(std::move(pred)));
}

struct SqlQuery::AutoFinish {
    AutoFinish(AutoFinish&&);
    AutoFinish& operator=(AutoFinish&&);
    ~AutoFinish();
private:
    friend class SqlQuery;
    SqlQuery*       q;
    AutoFinish(SqlQuery* _q);
};


