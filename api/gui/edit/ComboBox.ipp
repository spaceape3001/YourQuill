////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

ComboBox::ComboBox(QWidget* parent) : QComboBox(parent)
{
}

ComboBox::~ComboBox()
{
}

void    ComboBox::reset()
{
    setCurrentIndex(-1);
}


