////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Delegate.hpp"

class UIntegerDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(UIntegerDelegate, Delegate)
public:
    using DataType  = unsigned int;
    UIntegerDelegate(QObject* parent=nullptr);
    virtual ~UIntegerDelegate();
    
    virtual QVariant    render(const QVariant&) const override;
    virtual Compare     compare(const QVariant&, const QVariant&) const override; 
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const override;
    virtual bool        setEditorData(QWidget*, const QVariant&) const override;
    virtual QVariant    getEditorData(const QWidget*) const override;
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const override;

    unsigned int        minValue() const { return m_minValue; }
    unsigned int        maxValue() const { return m_maxValue; }
    unsigned int        step() const { return m_step; }
    
public slots:
    void                setMinValue(unsigned int);
    void                setMaxValue(unsigned int);
    void                setStep(unsigned int);
    void                setZeroNull(bool);
    
private:
    unsigned int        m_minValue, m_maxValue, m_step;
    bool                m_zeroNull;
};
