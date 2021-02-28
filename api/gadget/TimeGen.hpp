////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QDialog>

class QDateEdit;
class QLineEdit;
class QTimeEdit;


class TimeGen : public QDialog {
    Q_OBJECT
public:

    TimeGen(QWidget* parent=nullptr);
    ~TimeGen();

public slots:
    void                        generate();
private:
    QTimeEdit*                  m_start;
    QTimeEdit*                  m_end;
    QLineEdit*                  m_result;
};

