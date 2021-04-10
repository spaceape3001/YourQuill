////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

StringComboDelegate::StringComboDelegate(QObject* parent) : Delegate(parent), m_freeEdit(true)
{
}

StringComboDelegate::~StringComboDelegate()
{
}

Compare     StringComboDelegate::compare(const QVariant&a, const QVariant&b) const 
{
    return compare_igCase(a.toString(), b.toString());
}

QWidget*    StringComboDelegate::createEditor(QWidget* parent) const 
{
    ComboBox*  cb  = new ComboBox(parent);
    cb -> addItems(m_suggest);
    cb -> setEditable(m_freeEdit);
    return cb;
}

bool        StringComboDelegate::setEditorData(QWidget*wid, const QVariant&var) const 
{
    static_cast<ComboBox*>(wid) -> setCurrentText(var.toString());
    return true;
}

QVariant    StringComboDelegate::getEditorData(const QWidget* wid) const 
{
    return static_cast<const ComboBox*>(wid) -> currentText();
}

void        StringComboDelegate::connectEditedSignal(QWidget* editor, QObject* other, const char*slot) const 
{
    connect(editor, SIGNAL(currentTextChanged(const QString&)), other, slot);
}

void        StringComboDelegate::setFreeEdit(bool f)
{
    m_freeEdit      = f;
}

void        StringComboDelegate::setSuggest(const QStringList&list)
{
    m_suggest       = list;
    m_suggest.sort(Qt::CaseInsensitive);
}

