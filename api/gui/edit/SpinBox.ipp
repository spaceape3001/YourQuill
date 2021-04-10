////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

SpinBox::SpinBox(QWidget*parent) : QSpinBox(parent)
{
}

SpinBox::~SpinBox()
{
}

std::optional<unsigned int> SpinBox::getUInt(unsigned int z) const
{
    std::optional<unsigned int> ret;
    int v   = value();
    if(v != (int)z)
        ret     = v;
    return ret;
}

void                        SpinBox::set(const std::optional<unsigned int>&v, unsigned int z)
{
    setValue((int)(v ? *v : z));
}

void    SpinBox::reset()
{
    setValue(0);
}

