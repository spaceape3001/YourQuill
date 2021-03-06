////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/QuillData.hpp>
#include <gui/model/StdTableModel.hpp>
#include <gui/undo/UndoBase.hpp>

#include <QGroupBox>
#include <QAbstractTableModel>
#include <QTableView>


class QAction;
class QToolBar;

class QuillDirTable : public QGroupBox, public UndoBase {
    Q_OBJECT
public:
    
    class Model;
    class View;

    QuillDirTable(bool, const QString& title=QString(), QWidget*parent=nullptr);
    ~QuillDirTable();
    
    void                get(Vector<QuillData::Root>&) const;
    void                set(const Vector<QuillData::Root>&);
    Vector<QString>     dirs() const;
    
signals:
    void        cmdAdd();
    
public slots:
    void        addDir(const QString&);
    void        clear();
    void        setReadOnly(bool);
    
private slots:
    void        cmdRemove();
    void        cmdMoveDown();
    void        cmdMoveUp();
    
private:
    QToolBar*       m_bar;
    Model*          m_model;
    View*           m_view;
};

class QuillDirTable::Model : public StdTableModel<QuillData::Root> {
    Q_OBJECT
public:

    Model(bool, QObject*parent=nullptr);
    ~Model();
    
    Vector<QString> dirs() const;
    void            get(Vector<QuillData::Root>&) const;
    void            set(const Vector<QuillData::Root>&);
    
    
public slots:
    void        setReadOnly(bool f) { Base::setReadOnly(f); }
    void        clear() { Base::clear(); }
};


class QuillDirTable::View : public QTableView {
    Q_OBJECT
public:
    View(Model*);
    ~View();
    
    int         selectedRow() const;
    
private:
    Model*  m_model;
};

