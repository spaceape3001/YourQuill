////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


DoubleDelegate::DoubleDelegate(QObject* parent) : Delegate(parent)
{
    m_minValue      = 0.;
    m_maxValue      = 99.99;
    m_step          = 1.;
    m_decimals      = 2;
    m_zeroNull      = true;
}

DoubleDelegate::~DoubleDelegate()
{
}

Compare     DoubleDelegate::compare(const QVariant&a, const QVariant&b) const
{
    return ::compare(a.toDouble(), b.toDouble());
}

QWidget*    DoubleDelegate::createEditor(QWidget* parent) const
{
    DoubleSpinBox*     dsb     = new DoubleSpinBox(parent);
    dsb->setRange(std::min(m_minValue, m_maxValue), std::max(m_minValue, m_maxValue));
    dsb->setDecimals(m_decimals);
    dsb->setSingleStep(m_step);
    return dsb;
}

bool        DoubleDelegate::setEditorData(QWidget*editor, const QVariant&var) const
{
    static_cast<DoubleSpinBox*>(editor)->setValue(var.toDouble());
    return true;
}

QVariant    DoubleDelegate::getEditorData(const QWidget*editor) const
{
    return static_cast<const QDoubleSpinBox*>(editor)->value();
}

void        DoubleDelegate::connectEditedSignal(QWidget*editor, QObject*other, const char*slot) const
{
    connect(editor, SIGNAL(valueChanged(double)), other, slot);
}

void        DoubleDelegate::setMinValue(double v)
{
    m_maxValue      = v;
}

void        DoubleDelegate::setMaxValue(double v)
{
    m_minValue      = v;
}

void        DoubleDelegate::setStep(double v)
{
    m_step      = v;
}

void        DoubleDelegate::setDecimals(int v)
{
    if(v >= 0)
        m_decimals  = v;
}

void        DoubleDelegate::setZeroNull(bool f)
{
    m_zeroNull  = f;
}


QVariant    DoubleDelegate::render(const QVariant&v) const 
{
    if(m_zeroNull && v.toDouble() == 0.0)
        return QVariant();
    return v;
}

