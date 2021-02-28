////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QWidget>
#include "gui/UndoBase.hpp"

class CheckBox;
class DateEdit;
class QDate;


class DateWidget : public QWidget, public UndoBase {
    Q_OBJECT
public:

    DateWidget(QWidget*parent=nullptr);
    ~DateWidget();
    
    QDate       date() const;
    void        setDate(const QDate&);

    void            setDisplayFormat(const QString&);
    virtual void    setUndoCallback(UndoCallback*) override;

private slots:
    void        stateChanged(int);

signals:
    void        editingFinished();
private:

    CheckBox*   m_good;
    DateEdit*   m_date;    
    bool        m_null;
};

