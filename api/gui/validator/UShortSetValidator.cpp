////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "UShortSetValidator.hpp"

//bool     UShortSetValidator::isValid(const QString&, int* pos=nullptr);

UShortSetValidator::UShortSetValidator(QObject*parent) : QValidator(parent)
{
}

UShortSetValidator::~UShortSetValidator()
{
}


namespace {
    QString reduce(const QString& input)
    {
        QString ret;
        bool    sp  = true;
        for(QChar ch : input){
            if(ch.isDigit()){
                sp  = false;
                ret += ch;
            } else if(ch == ' '){
                if(sp)
                    continue;
                ret += ' ';
                sp = true;
            }
        }
        return ret;
    }
}

void                UShortSetValidator::fixup(QString&input) const 
{
    input       = reduce(input);
}

QValidator::State   UShortSetValidator::validate(QString& input, int& pos) const 
{
    input       = reduce(input);
    if(input.isEmpty())
        return Acceptable;
        
    //  TODO (Range checking);
    return Acceptable;
}


#include "moc_UShortSetValidator.cpp"
