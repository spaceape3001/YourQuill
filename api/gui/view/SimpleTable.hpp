////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractTableModel>
#include <QTableView>
#include <QVariant>
#include <util/Array2.hpp>
#include <util/Vector.hpp>


/*! \brief Simple Qt table

    Meant for a fast view of data
*/
class  SimpleTable : public QTableView {
    Q_OBJECT
public:
    SimpleTable(QWidget*parent=nullptr);

    bool    set(int,int,const QVariant&);
    void    setHeader(const Vector<QString>&);

    class Model;
    Model*  model() { return m_model; }
    const Model* model() const { return m_model; }

public slots:
    void        clear();
//    void        fit();

private:
    Model*      m_model;
};

class SimpleTable::Model : public QAbstractTableModel {
    Q_OBJECT
public:

    Model(QObject* parent=nullptr);
    ~Model();

    void        setHeader(const Vector<QString>&);
    int         rowCount(const QModelIndex&) const { return m_data.rowCount(); }
    int         columnCount(const QModelIndex&) const { return m_data.columnCount(); }
    QVariant    data(const QModelIndex&, int) const;
    QVariant    headerData(int, Qt::Orientation, int) const;

    bool        set(int,int,const QVariant&);

public slots:
    void        clear();

private:
    Vector<QString>     m_headers;
    Array2<QVariant>    m_data;
};
