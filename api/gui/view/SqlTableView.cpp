////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SqlTableView.hpp"
#include <QSqlTableModel>

SqlTableView::SqlTableView(QSqlDatabase db, QWidget*parent) : QTableView(parent), m_model(nullptr)
{
    m_model = new QSqlTableModel(this, db);
    setModel(m_model);
    setSortingEnabled(true);
    setAlternatingRowColors(true);
}

SqlTableView::~SqlTableView()
{
}

void            SqlTableView::setTable(const QString& s)
{
    m_model -> setTable(s);
}

#include "moc_SqlTableView.cpp"

