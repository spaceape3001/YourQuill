////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

UShortSetEdit::UShortSetEdit(QWidget*parent) : LineEdit(parent)
{
    setValidator(new UShortSetValidator(this));
}

UShortSetEdit::~UShortSetEdit()
{
}

Set<uint16_t>       UShortSetEdit::get() const
{
    Set<uint16_t>   ret;
    QString     line    = text();
    if(!line.isEmpty()){
        for(QString s : line.split(' ')){
            s   = s.trimmed();
            if(s.isEmpty())
                continue;
            bool    ok  = false;
            uint16_t    n   = s.toUShort(&ok);
            if(!ok)
                continue;
            ret << n;
        }
    }
    return ret;
}

void                UShortSetEdit::set(const Set<uint16_t>& them)
{
    setText(join_qstring(them, " "));
}

