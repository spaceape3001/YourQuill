////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QDialog>

class QDateEdit;
class QLineEdit;


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


