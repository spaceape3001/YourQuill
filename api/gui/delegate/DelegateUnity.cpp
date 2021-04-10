////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#include "DelegateImpl.hpp"

#include "ColorComboDelegate.hpp"
#include "ColorDelegate.hpp"
#include "DateDelegate.hpp"
#include "DateTimeDelegate.hpp"
#include "Delegate.hpp"
#include "DirDelegate.hpp"
#include "DoubleDelegate.hpp"
#include "EnumDelegate.hpp"
#include "FileDelegate.hpp"
#include "IconBarDelegate.hpp"
#include "ImageDelegate.hpp"
#include "IntegerDelegate.hpp"
#include "StringComboDelegate.hpp"
#include "StringDelegate.hpp"
#include "UIntegerDelegate.hpp"


#include <gui/edit/ColorCombo.hpp>
#include <gui/edit/ComboBox.hpp>
#include <gui/edit/DateTimeWidget.hpp>
#include <gui/edit/DateWidget.hpp>
#include <gui/edit/DirBox.hpp>
#include <gui/edit/DoubleSpinBox.hpp>
#include <gui/edit/FileBox.hpp>
#include <gui/edit/GenericEnumCombo.hpp>
#include <gui/edit/LineEdit.hpp>
#include <gui/edit/SpinBox.hpp>

#include <QColorDialog>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>


#include <util/Compare.hpp>
#include <util/Map.hpp>
#include <util/Vector.hpp>


namespace {
    QColor  colorFor(const QVariant&v){
        if((QMetaType::Type) v.type() == QMetaType::QColor)
            return v.value<QColor>();
        return QColor(v.toString());
    }
}

#include "ColorComboDelegate.ipp"
#include "ColorDelegate.ipp"
#include "DateDelegate.ipp"
#include "DateTimeDelegate.ipp"
#include "Delegate.ipp"
#include "DirDelegate.ipp"
#include "DoubleDelegate.ipp"
#include "EnumDelegate.ipp"
#include "FileDelegate.ipp"
#include "IconBarDelegate.ipp"
#include "ImageDelegate.ipp"
#include "IntegerDelegate.ipp"
#include "StringComboDelegate.ipp"
#include "StringDelegate.ipp"
#include "UIntegerDelegate.ipp"

MO_ABSTRACT(Delegate)
MO_ABSTRACT(GenericEnumDelegate)

MO_CONCRETE(ColorComboDelegate)
MO_CONCRETE(ColorDelegate)
MO_CONCRETE(DateDelegate)
MO_CONCRETE(DateTimeDelegate)
MO_CONCRETE(DirDelegate)
MO_CONCRETE(DoubleDelegate)
MO_CONCRETE(FileDelegate)
MO_CONCRETE(IconBarDelegate)
MO_CONCRETE(ImageDelegate)
MO_CONCRETE(IntegerDelegate)
MO_CONCRETE(StringComboDelegate)
MO_CONCRETE(StringDelegate)
MO_CONCRETE(UIntegerDelegate)


#include "moc_ColorComboDelegate.cpp"
#include "moc_ColorDelegate.cpp"
#include "moc_DateDelegate.cpp"
#include "moc_DateTimeDelegate.cpp"
#include "moc_Delegate.cpp"
#include "moc_DirDelegate.cpp"
#include "moc_DoubleDelegate.cpp"
#include "moc_EnumDelegate.cpp"
#include "moc_FileDelegate.cpp"
#include "moc_IconBarDelegate.cpp"
#include "moc_ImageDelegate.cpp"
#include "moc_IntegerDelegate.cpp"
#include "moc_StringComboDelegate.cpp"
#include "moc_StringDelegate.cpp"
#include "moc_UIntegerDelegate.cpp"
