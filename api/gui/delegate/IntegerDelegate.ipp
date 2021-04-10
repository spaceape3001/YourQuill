////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

IntegerDelegate::IntegerDelegate(QObject* parent) : Delegate(parent)
{
    m_minValue  = 0;
    m_maxValue  = 999;
    m_step      = 1;
    m_zeroNull  = true;
}

IntegerDelegate::~IntegerDelegate()
{
}


Compare     IntegerDelegate::compare(const QVariant&va, const QVariant&vb) const 
{
    return ::compare(va.toInt(), vb.toInt());
}

QWidget*    IntegerDelegate::createEditor(QWidget* parent) const
{
    SpinBox*       sb  = new SpinBox(parent);
    sb->setRange(std::min(m_minValue, m_maxValue), std::max(m_minValue, m_maxValue));
    sb->setSingleStep(m_step);
    return sb;
}

bool        IntegerDelegate::setEditorData(QWidget*editor, const QVariant&val) const
{
    static_cast<SpinBox*>(editor)->setValue( val.toInt());
    return true;
}

QVariant    IntegerDelegate::getEditorData(const QWidget*editor) const
{
    return static_cast<const SpinBox*>(editor)->value();
}

void        IntegerDelegate::connectEditedSignal(QWidget*editor, QObject*other, const char*slot) const
{
    connect(editor, SIGNAL(valueChanged(int)), other, slot);
}

void        IntegerDelegate::setMaxValue(int v)
{
    m_maxValue      = v;
}

void        IntegerDelegate::setMinValue(int v)
{
    m_minValue      = v;
}

void        IntegerDelegate::setStep(int v)
{
    m_step          = v;
}

void        IntegerDelegate::setZeroNull(bool f) 
{
    m_zeroNull      = f;
}


QVariant    IntegerDelegate::render(const QVariant&v) const 
{
    if(m_zeroNull && v.toInt() == 0)
        return QVariant();
    return v;
}



