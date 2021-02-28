////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QTableView>

class QSqlDatabase;
class QSqlTableModel;

class SqlTableView : public QTableView {
    Q_OBJECT
public:
    SqlTableView(QSqlDatabase, QWidget*parent=nullptr);
    ~SqlTableView();

public slots:
    void            setTable(const QString&);

private:
    QSqlTableModel* m_model;
};
