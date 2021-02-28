////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QValidator>

class KeyValidator : public QValidator {
    Q_OBJECT
public:
    KeyValidator(QObject*parent=nullptr);
    ~KeyValidator();
    
    void                fixup(QString&input) const override;
    QValidator::State   validate(QString& input, int& pos) const override;
};
