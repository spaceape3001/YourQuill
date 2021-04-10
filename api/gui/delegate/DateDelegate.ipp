////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


DateDelegate::DateDelegate(QObject*parent) : Delegate(parent)
{
}

DateDelegate::~DateDelegate()
{
}
    
Compare     DateDelegate::compare(const QVariant&va, const QVariant&vb) const
{
    return compare(va.toDate(), vb.toDate());
}

QWidget*    DateDelegate::createEditor(QWidget* parent) const
{
    return new DateWidget(parent);
}

bool        DateDelegate::setEditorData(QWidget*editor, const QVariant&val) const
{
    static_cast<DateWidget*>(editor)->setDate(val.toDate());
    return true;
}

QVariant    DateDelegate::getEditorData(const QWidget*editor) const
{
    return static_cast<const DateWidget*>(editor)->date();
}

void        DateDelegate::connectEditedSignal(QWidget*editor, QObject*obj, const char*slot) const
{
    connect(editor, SIGNAL(editingFinished()), obj, slot);
}

QVariant    DateDelegate::render(const QVariant&input) const 
{
    QDate d =  input.value<QDate>();
    if(d.isNull())
        return QVariant();
    return d.toString(Qt::ISODate);
}


