////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "XmlIdentifierValidator.hpp"

XmlIdentifierValidator::XmlIdentifierValidator(QObject*parent) : QValidator(parent)
{
}

XmlIdentifierValidator::~XmlIdentifierValidator()
{
}

void                XmlIdentifierValidator::fixup(QString&input) const
{
    input.replace(" ", ""); // strip spaces
}

bool     XmlIdentifierValidator::isValid(const QString& input, int *pos)
{
    int     i   = 0;
    if(!input.at(0).isLetter()){
        if(pos)
            *pos  = 0;
        return false;
    } 
    
    if((input.at(0).toLower() == 'x') && (input.at(1).toLower() == 'm') && (input.at(2).toLower() == 'l')){
        if(pos)
            pos     = 0;
        return false;
    }
    
    while( ++i < input.size()){
        QChar   ch  = input.at(i);
        if(ch.isLetterOrNumber() || ch == '_')
            continue;
        if(pos)
            *pos     = i;
        return false;
    }
    return true;
}

QValidator::State   XmlIdentifierValidator::validate(QString& input, int& pos) const
{
    if(input.isEmpty())
        return Intermediate;
    return isValid(input, &pos) ? Acceptable : Invalid;
}


#include "moc_XmlIdentifierValidator.cpp"
