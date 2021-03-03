////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Delegate.hpp"

class StringDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(StringDelegate, Delegate)
public:
    using DataType      = QString;

    //virtual QVariant    render(const QVariant&) const override;
    virtual Compare     compare(const QVariant&, const QVariant&) const override; 
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const override;
    virtual bool        setEditorData(QWidget*, const QVariant&) const override;
    virtual QVariant    getEditorData(const QWidget*) const override;
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const override;

    StringDelegate(QObject*parent=nullptr);
    StringDelegate(QValidator*, QObject*parent=nullptr);
    virtual ~StringDelegate();
private:
    QValidator*         m_validator;
};
