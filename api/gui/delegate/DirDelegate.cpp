////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirDelegate.hpp"
#include "DelegateImpl.hpp"
#include <gui/edit/DirBox.hpp>

DirDelegate::DirDelegate(QObject*parent) : Delegate(parent)
{
}

DirDelegate::~DirDelegate()
{
}

Compare     DirDelegate::compare(const QVariant&a, const QVariant&b) const
{
    return ::compare(a.toString(), b.toString());
}

QWidget*    DirDelegate::createEditor(QWidget* parent) const
{
    return new DirBox(parent);
}

bool        DirDelegate::setEditorData(QWidget*a, const QVariant&var) const
{
    static_cast<DirBox*>(a) -> setDirPath(var.toString());
    return true;
}
    
QVariant    DirDelegate::getEditorData(const QWidget*w) const
{
    return static_cast<const DirBox*>(w) -> dirPath();
}

//virtual bool        paint(QPainter*, const QStyleOptionViewItem& option, const QVariant&) const;
void        DirDelegate::connectEditedSignal(QWidget*editor, QObject*other, const char*slot) const
{
    connect(editor, SIGNAL(dirPathChanged(const QString&)), other, slot);
}

MO_CONCRETE(DirDelegate)

#include "moc_DirDelegate.cpp"
