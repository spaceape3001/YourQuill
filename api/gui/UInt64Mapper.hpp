////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <QObject>
#include "util/Hash.hpp"

class UInt64Mapper : public QObject {
    Q_OBJECT
public:
    UInt64Mapper(QObject*parent=nullptr);
    ~UInt64Mapper();
    void        setMapping(QObject*, uint64_t );
    
public slots:
    void        map();
    
signals:
    void        mapped(quint64);
private:
    Hash<QObject*,quint64>   m_wid2u64;
};
