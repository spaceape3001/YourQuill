////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Dock.hpp"
#include <QDialog>

class QDateEdit;
class QSpinBox;

class Easter : public QDialog {
    Q_OBJECT
public:

    Easter(QWidget* parent=nullptr);
    ~Easter();
    
private slots:
    void        yearChange();

private:
    QDateEdit*      m_date  = nullptr;
    QSpinBox*       m_year  = nullptr;
};

class EasterDock : public Dock {
    Q_OBJECT
public:
    EasterDock(QWidget*parent=nullptr);
    ~EasterDock();
};

