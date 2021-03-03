////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Delegate.hpp"

class StringComboDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(StringComboDelegate, Delegate)
public:

    StringComboDelegate(QObject* parent=nullptr);
    virtual ~StringComboDelegate();
    
    virtual Compare     compare(const QVariant&, const QVariant&) const override; 
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const override;
    virtual bool        setEditorData(QWidget*, const QVariant&) const override;
    virtual QVariant    getEditorData(const QWidget*) const override;
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const override;

    const QStringList&  suggest() const { return m_suggest; }
    bool                freeEdit() const { return m_freeEdit; }
public slots:
    void                setFreeEdit(bool);
    void                setSuggest(const QStringList&);
signals:
    void                newEntry(const QString&);
private:
    QStringList         m_suggest;
    bool                m_freeEdit;
};

