////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QValidator>


class NoSpacesValidator : public QValidator {
    Q_OBJECT
public:    
    NoSpacesValidator(QObject*parent=nullptr);
    ~NoSpacesValidator();

    void                fixup(QString&input) const override;
    QValidator::State   validate(QString& input, int& pos) const override;
};
