////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

KeyValidator::KeyValidator(QObject*parent) : QValidator(parent)
{
}

KeyValidator::~KeyValidator()
{
}

void                KeyValidator::fixup(QString&input) const
{
    input.replace(" ", ""); // strip spaces
}

QValidator::State   KeyValidator::validate(QString& input, int& pos) const
{
    if(input.isEmpty())
        return Intermediate;
    
    int     i   = 0;
    if(!input.at(0).isLetterOrNumber()){
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

