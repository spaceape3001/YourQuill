////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ColorDelegate.hpp"
#include "DelegateImpl.hpp"
#include <QColorDialog>
#include <QPainter>

namespace {
    QColor  colorFor(const QVariant&v){
        if((QMetaType::Type) v.type() == QMetaType::QColor)
            return v.value<QColor>();
        return QColor(v.toString());
    }
}


ColorDelegate::ColorDelegate(QObject* parent) : Delegate(parent)
{
}

ColorDelegate::~ColorDelegate()
{
}


QWidget*    ColorDelegate::createEditor(QWidget* parent) const
{
    return new QColorDialog(parent);
}

bool        ColorDelegate::setEditorData(QWidget*editor, const QVariant&val) const
{
    QColorDialog*       cc  = static_cast<QColorDialog*>(editor);
    QColor      v   = colorFor(val);
    if(v != cc->currentColor())
        cc->setCurrentColor(v);
    return true;
}

QVariant    ColorDelegate::getEditorData(const QWidget*editor) const
{
    return static_cast<const QColorDialog*>(editor)->currentColor();
}

bool        ColorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& val) const
{
    // update to handle strings on value.....
    QColor  color   = colorFor(val);
    if(!color.isValid()){
        painter->drawLine(QLine(option.rect.left(), option.rect.top(), option.rect.right(), option.rect.bottom()));
        painter->drawLine(QLine(option.rect.left(), option.rect.bottom(), option.rect.right(), option.rect.top()));
    } else
        painter->fillRect(option.rect, color);
    return true;
}

void        ColorDelegate::connectEditedSignal(QWidget*editor, QObject*other, const char*slot) const
{
    connect(editor, SIGNAL(currentColorChanged(const QColor&)), other, slot);
    connect(editor, SIGNAL(colorSelected(const QColor&)), other, slot);
}


MO_CONCRETE(ColorDelegate)

#include "moc_ColorDelegate.cpp"
