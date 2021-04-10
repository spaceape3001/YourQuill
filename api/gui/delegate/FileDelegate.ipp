////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

FileDelegate::FileDelegate(QObject*parent) : Delegate(parent)
{
}

FileDelegate::~FileDelegate()
{
}

Compare     FileDelegate::compare(const QVariant&a, const QVariant&b) const
{
    return ::compare(a.toString(), b.toString());
}

QWidget*    FileDelegate::createEditor(QWidget* parent) const
{
    return new FileBox(parent);
}

bool        FileDelegate::setEditorData(QWidget*a, const QVariant&var) const
{
    static_cast<FileBox*>(a) -> setFile(var.toString());
    return true;
}
    
QVariant    FileDelegate::getEditorData(const QWidget*w) const
{
    return static_cast<const FileBox*>(w) -> file();
}

//virtual bool        paint(QPainter*, const QStyleOptionViewItem& option, const QVariant&) const;
void        FileDelegate::connectEditedSignal(QWidget*editor, QObject*other, const char*slot) const
{
    connect(editor, SIGNAL(fileChanged(const QString&)), other, slot);
}




