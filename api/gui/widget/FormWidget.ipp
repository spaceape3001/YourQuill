////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

FormWidget::FormWidget(QWidget*parent) : QWidget(parent)
{
    m_layout        = new QFormLayout;
    setLayout(m_layout);
}

FormWidget::~FormWidget()
{
}

void    FormWidget::addRow(QWidget* label, QWidget* wid)
{
    m_layout -> addRow(label, wid);
    check(label);
    check(wid);
}


void    FormWidget::addRow(QWidget*label, QLayout*lay)
{
    m_layout -> addRow(label, lay);
    check(label);
    check(lay);
}

void    FormWidget::addRow(QWidget* label, std::initializer_list<QWidget*> widgets, bool fStretchLast)
{
    addRow(label, makeLayout(widgets, fStretchLast));
}


void    FormWidget::addRow(const QString&label, QWidget*wid)
{
    m_layout -> addRow(label, wid);
    check(wid);
}


void    FormWidget::addRow(const QString&label, std::initializer_list<QWidget*> widgets, bool fStretchLast)
{
    addRow(label, makeLayout(widgets, fStretchLast));
}

void    FormWidget::addRow(const QString&label, QLayout* lay)
{
    m_layout -> addRow(label, lay);
    check(lay);
}

void    FormWidget::addRow(QLayout*lay)
{
    m_layout -> addRow(lay);
    check(lay);
}

void    FormWidget::addRow(QWidget*wid)
{
    m_layout -> addRow(wid);
    check(wid);
}

void    FormWidget::addSeparator(QFrame::Shadow s)
{
    QFrame* f   = new QFrame;
    f -> setFrameShadow(s);
    f -> setLineWidth(2);
    m_layout->addWidget(f);
}

void    FormWidget::check(QWidget*wid)
{
    UndoBase*   ub  = dynamic_cast<UndoBase*>(wid);
    if(ub){
        if(callback())
            ub -> setUndoCallback(callback());
        m_subUndos << ub;
    }
}

void    FormWidget::check(QLayout*lay)
{
    if(lay){
        int cnt  = lay -> count();
        for(int i = 0; i<cnt; ++i){
            QLayoutItem*    it  = lay ->itemAt(i);
            if(it -> layout())
                check(it -> layout());
            if(it -> widget())
                check(it -> widget());
        }
    }
}


void    FormWidget::setUndoCallback(UndoCallback*callback) 
{
    UndoBase::setUndoCallback(callback);
    for(UndoBase* ub : m_subUndos)
        ub -> setUndoCallback(callback);
}

