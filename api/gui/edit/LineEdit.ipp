////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


LineEdit::LineEdit(QWidget* parent) : QLineEdit(parent)
{
    connect(this, &LineEdit::textEdited, this, &LineEdit::createUndo);
}

LineEdit::~LineEdit()
{
}

void    LineEdit::createUndo()
{
    //  TODO
}

std::optional<QString>  LineEdit::get() const
{
    std::optional<QString>  ret;
    QString                 t   = text();
    if(!t.isEmpty())
        ret = t;
    return t;
}

void                    LineEdit::set(const std::optional<QString>&v)
{
    if(v){
        setText(*v);
    } else
        clear();
}

