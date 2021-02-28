////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ColorCombo.hpp"
#include <QColor>

ColorCombo::ColorCombo(QWidget* parent) : ComboBox(parent)
{
    populate();
}

ColorCombo::~ColorCombo()
{
}

QColor  ColorCombo::color() const
{
    return itemData(currentIndex(), Qt::DecorationRole).value<QColor>();
}

void    ColorCombo::setColor(QColor clr)
{
    setCurrentIndex(findData(clr, (int) Qt::DecorationRole));
}
    

void    ColorCombo::populate()
{
    insertItem(0, "(no color)");
    setItemData(0, QColor(), Qt::DecorationRole);
    QStringList     names   = QColor::colorNames();
    names.sort();
    for(int i=0;i<names.size();++i){
        insertItem(i+1, names[i]);
        setItemData(i+1, QColor(names[i]), Qt::DecorationRole);
    }
}



#include "moc_ColorCombo.cpp"
