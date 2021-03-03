////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Delegate.hpp"
#include <util/Enum.hpp>

/*! \brief Enumeration Delegate

    The render/create can be any of integer, string, and/or the enumeration.
    The setData will be in the enum type.
*/
class GenericEnumDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(GenericEnumDelegate, Delegate)
public:
    GenericEnumDelegate(const EnumDef*, int type, QObject *parent=nullptr);
    ~GenericEnumDelegate();
    
    virtual QVariant    render(const QVariant&) const override;
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const;
    virtual bool        setEditorData(QWidget*, const QVariant&) const;
    virtual QVariant    getEditorData(const QWidget*) const;
    //virtual bool        paint(QPainter*, const QStyleOptionViewItem& option, const QVariant&) const;
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const;
    virtual QString 	displayText(const QVariant &value, const QLocale &locale) const override;    

private:
    const EnumDef*   m_enum;
    int              m_type;
};

template <typename E>
class EnumDelegate : public GenericEnumDelegate {
public:
    using DataType = E;

    EnumDelegate(QObject*parent=nullptr) : GenericEnumDelegate( E::staticEnumInfo(), qMetaTypeId<E>(), parent ) 
    {
    }
    
    virtual ~EnumDelegate()
    {
    }
};
