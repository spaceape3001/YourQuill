////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "EnumDelegate.hpp"
#include "DelegateImpl.hpp"
#include <gui/edit/GenericEnumCombo.hpp>

GenericEnumDelegate::GenericEnumDelegate(const EnumDef*def, int type, QObject *parent) : 
    Delegate(parent), m_enum(def), m_type(type)
{
}

GenericEnumDelegate::~GenericEnumDelegate()
{
}

void        GenericEnumDelegate::connectEditedSignal(QWidget* editor, QObject* other, const char* slot) const
{
    connect(editor, SIGNAL(fileChanged(const QString&)), other, slot);
}

QWidget*    GenericEnumDelegate::createEditor(QWidget* parent) const
{
    return new GenericEnumCombo(m_enum, parent);
}

QString 	GenericEnumDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    return render(value).toString();
}

QVariant    GenericEnumDelegate::getEditorData(const QWidget* wid) const
{
    int i   = static_cast<const GenericEnumCombo*>(wid) -> intValue();
    return QVariant(m_type, &i);
}

QVariant    GenericEnumDelegate::render(const QVariant&var) const 
{
    if((var.userType() == m_type ) || var.userType() == QMetaType::Int){
        int i   = *((const int*) var.constData());
        String txt = m_enum -> key_of(i);
        return txt.qString();
    } else
        return var;
}

bool        GenericEnumDelegate::setEditorData(QWidget*wid, const QVariant&val) const
{
    if((val.userType() == m_type ) || val.userType() == QMetaType::Int){
        static_cast<GenericEnumCombo*>(wid) -> setIntValue(*((const int*) val.constData()));
        return true;
    } else if(val.userType() == QMetaType::QString){
        int_r  i   = m_enum -> value_of( val.toString());
        if(i.good)
            static_cast<GenericEnumCombo*>(wid) -> setIntValue(i.value);
        return i.good;
    } else
        return false;
}

//bool        GenericEnumDelegate::paint(QPainter*painter, const QStyleOptionViewItem& option, const QVariant&var) const
//{
    //if((var.userType() == m_type ) || var.userType() == QMetaType::Int){
        //int i   = *((const int*) var.constData());
        //QString txt = m_enum -> keyOf(i);
        //return Delegate::paint(painter, option, txt);
    //} else
        //return Delegate::paint(painter, option, var);
//}


MO_ABSTRACT(GenericEnumDelegate)

#include "moc_EnumDelegate.cpp"
