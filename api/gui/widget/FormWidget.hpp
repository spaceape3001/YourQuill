////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <gui/undo/UndoBase.hpp>
#include <util/Vector.hpp>
#include <QWidget>
#include <QFrame>

class QFormLayout;

class FormWidget : public QWidget, public UndoBase {
    Q_OBJECT
public:

    FormWidget(QWidget*parent=nullptr);
    virtual ~FormWidget();

    void    addRow(QWidget*, QWidget*);
    void    addRow(QWidget*, std::initializer_list<QWidget*>, bool fStretchLast=false);
    void    addRow(QWidget*, QLayout*);
    void    addRow(const QString&, QWidget*);
    void    addRow(const QString&, std::initializer_list<QWidget*>, bool fStretchLast=false);
    void    addRow(const QString&, QLayout*);
    //void    addRow(const QString&, std::initializer_list<QWidget*>);
    
    void    addRow(QLayout*);
    void    addRow(QWidget*);
    void    addSeparator(QFrame::Shadow s = QFrame::Plain);
    
    void    setUndoCallback(UndoCallback*) override;
    
    QFormLayout*        layout() const { return m_layout; }

private:
    QFormLayout*        m_layout;
    Vector<UndoBase*>   m_subUndos;
    
    void    check(QWidget*);
    void    check(QLayout*);
};
