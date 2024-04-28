////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Dock.hpp"
#include <QDialog>

class QDateEdit;
class QLineEdit;


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


class DateGen : public QDialog {
    Q_OBJECT
public:
    DateGen(QWidget* parent=nullptr);
    ~DateGen();

public slots:
    void                                generate();

private:
    QDateEdit*                          m_start;
    QDateEdit*                          m_end;
    QLineEdit*                          m_result;
};

class DateGenDock : public Dock {
    Q_OBJECT
public:
    DateGenDock(QWidget*parent=nullptr);
    ~DateGenDock();
};

