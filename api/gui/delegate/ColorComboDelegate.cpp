////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ColorComboDelegate.hpp"
#include "DelegateImpl.hpp"
#include <gui/edit/ColorCombo.hpp>
#include <QPainter>

namespace {
    QColor  colorFor(const QVariant&v){
        if((QMetaType::Type) v.type() == QMetaType::QColor)
            return v.value<QColor>();
        return QColor(v.toString());
    }
}

ColorComboDelegate::ColorComboDelegate(QObject* parent) : Delegate(parent)
{
}

ColorComboDelegate::~ColorComboDelegate()
{
}


QWidget*    ColorComboDelegate::createEditor(QWidget* parent) const
{
    return new ColorCombo(parent);
}

bool        ColorComboDelegate::setEditorData(QWidget*editor, const QVariant&val) const
{
    ColorCombo*       cc  = static_cast<ColorCombo*>(editor);
    QColor      v   = colorFor(val);
    if(v != cc->color())
        cc->setColor(v);
    return true;
}

QVariant    ColorComboDelegate::getEditorData(const QWidget*editor) const
{
    return static_cast<const ColorCombo*>(editor)->color();
}

bool        ColorComboDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& val) const
{
    QColor  color   = colorFor(val);
    if(!color.isValid()){
        painter->drawLine(QLine(option.rect.left(), option.rect.top(), option.rect.right(), option.rect.bottom()));
        painter->drawLine(QLine(option.rect.left(), option.rect.bottom(), option.rect.right(), option.rect.top()));
    } else
        painter->fillRect(option.rect, color);
    return true;
}

void        ColorComboDelegate::connectEditedSignal(QWidget*editor, QObject*other, const char*slot) const
{
    connect(editor, SIGNAL(currentTextChanged(int)), other, slot);
    connect(editor, SIGNAL(currentIndexChanged(int)), other, slot);
}

MO_CONCRETE(ColorComboDelegate)

#include "moc_ColorComboDelegate.cpp"
