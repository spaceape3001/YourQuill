////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QWidget>
#include "gui/undo/UndoBase.hpp"

class CheckBox;
class DateTimeEdit;
class QDateTime;

class DateTimeWidget : public QWidget, public UndoBase {
    Q_OBJECT
public:

    DateTimeWidget(QWidget*parent=nullptr);
    ~DateTimeWidget();
    
    QDateTime       dateTime() const;
    void            setDateTime(const QDateTime&);

    void            setDisplayFormat(const QString&);
    virtual void    setUndoCallback(UndoCallback*) override;

private slots:
    void        stateChanged(int);

signals:
    void        editingFinished();
private:

    CheckBox*       m_good;
    DateTimeEdit*   m_date;    
    bool            m_null;
};

