////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Delegate.hpp"

class FileDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(FileDelegate, Delegate)
public:
    FileDelegate(QObject*parent =nullptr);
    ~FileDelegate();
    virtual Compare     compare(const QVariant&, const QVariant&) const override; 
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const;
    virtual bool        setEditorData(QWidget*, const QVariant&) const;
    virtual QVariant    getEditorData(const QWidget*) const;
    //virtual bool        paint(QPainter*, const QStyleOptionViewItem& option, const QVariant&) const;
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const;
};



