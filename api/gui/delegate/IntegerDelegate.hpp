////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Delegate.hpp"


class IntegerDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(IntegerDelegate, Delegate)
public:
    using DataType      = int;
    IntegerDelegate(QObject* parent=nullptr);
    virtual ~IntegerDelegate();
    
    virtual QVariant    render(const QVariant&) const override;
    virtual Compare     compare(const QVariant&, const QVariant&) const override; 
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const override;
    virtual bool        setEditorData(QWidget*, const QVariant&) const override;
    virtual QVariant    getEditorData(const QWidget*) const override;
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const override;

    int                 minValue() const { return m_minValue; }
    int                 maxValue() const { return m_maxValue; }
    int                 step() const { return m_step; }
    bool                zeroNull() const { return m_zeroNull; }
    
public slots:
    void                setMinValue(int);
    void                setMaxValue(int);
    void                setStep(int);
    void                setZeroNull(bool);
    
private:
    int                 m_minValue, m_maxValue, m_step;
    bool                m_zeroNull;
};
