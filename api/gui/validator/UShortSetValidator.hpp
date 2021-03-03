////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QValidator>

/*! \brief Validaotr fora  line-edit of uhort strings
*/
class UShortSetValidator : public QValidator {
    Q_OBJECT
public:
    
    UShortSetValidator(QObject*parent=nullptr);
    ~UShortSetValidator();
    
    void                fixup(QString&input) const override;
    QValidator::State   validate(QString& input, int& pos) const override;
    
};

