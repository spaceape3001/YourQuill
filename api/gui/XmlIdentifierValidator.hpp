////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


#include <QValidator>


class XmlIdentifierValidator : public QValidator {
    Q_OBJECT
public:
    XmlIdentifierValidator(QObject*parent=nullptr);
    ~XmlIdentifierValidator();
    
    static bool     isValid(const QString&, int* pos=nullptr);
    
    void                fixup(QString&input) const override;
    QValidator::State   validate(QString& input, int& pos) const override;
};

