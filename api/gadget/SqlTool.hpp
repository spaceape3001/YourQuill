////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <QSqlQuery>

class QComboBox;
class QSqlQuery;
class QSqlQueryModel;
class QStackedLayout;
class QStatusBar;
class QTableView;
class QTextEdit;

class SqlTool : public SubWin {
    Q_OBJECT
public:
    SqlTool(QSqlDatabase, QWidget*parent=nullptr);
    ~SqlTool();
    
public slots:
    void        refresh();
private slots:
    void        submit();
    void        clear();
private:
    void                _exec();

    QSqlQueryModel*     m_model;
    QTableView*         m_view;
    QComboBox*          m_edit;
    QTextEdit*          m_error;
    QStackedLayout*     m_stack;
    QSqlQuery           m_query;
};
