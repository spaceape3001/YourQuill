////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

NoSpacesValidator::NoSpacesValidator(QObject*parent) : QValidator(parent)
{
}

NoSpacesValidator::~NoSpacesValidator()
{
}

void                NoSpacesValidator::fixup(QString&input) const
{
    input.replace(" ", "");
}

QValidator::State   NoSpacesValidator::validate(QString& input, int& pos) const
{
    if(input.isEmpty())
        return Intermediate;

    int     i   = 0;
    for(QChar ch : input){
        if(ch.isSpace()){
            pos     = i;
            return QValidator::Invalid;
        }
        ++i;
    }
    return QValidator::Acceptable;
}

