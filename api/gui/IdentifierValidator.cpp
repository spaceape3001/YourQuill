////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdentifierValidator.hpp"

IdentifierValidator::IdentifierValidator(QObject*parent) : QValidator(parent)
{
}

IdentifierValidator::~IdentifierValidator()
{
}

void                IdentifierValidator::fixup(QString&input) const
{
    input.replace(" ", ""); // strip spaces
}

QValidator::State   IdentifierValidator::validate(QString& input, int& pos) const
{
    if(input.isEmpty())
        return Intermediate;
    
    int     i   = 0;
    if(!input.at(0).isLetter()){
        pos     = 0;
        return Invalid;
    } 
    
    while( ++i < input.size()){
        QChar   ch  = input.at(i);
        if(ch.isLetterOrNumber() || ch == '_')
            continue;
        pos     = i;
        return Invalid;
    }
    return Acceptable;
}

#include "moc_IdentifierValidator.cpp"
