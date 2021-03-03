////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Delegate.hpp"

class DateTimeDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(DateTimeDelegate, Delegate)
public:

    using DataType  = QDateTime;
    
    DateTimeDelegate(QObject* parent=nullptr);
    virtual ~DateTimeDelegate();
    
    virtual QVariant    render(const QVariant&) const override;
    virtual Compare     compare(const QVariant&, const QVariant&) const override; 
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const override;
    virtual bool        setEditorData(QWidget*, const QVariant&) const override;
    virtual QVariant    getEditorData(const QWidget*) const override;
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const override;
};

