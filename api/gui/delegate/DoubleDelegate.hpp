////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Delegate.hpp"

class  DoubleDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(DoubleDelegate, Delegate)
public:

    using DataType  = double;
    DoubleDelegate(QObject* parent=nullptr);
    virtual ~DoubleDelegate();
    
    virtual QVariant    render(const QVariant&) const override;
    virtual Compare     compare(const QVariant&, const QVariant&) const override; 
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const override;
    virtual bool        setEditorData(QWidget*, const QVariant&) const override;
    virtual QVariant    getEditorData(const QWidget*) const override;
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const override;

    double      minValue() const { return m_minValue; }
    double      maxValue() const { return m_maxValue; }
    double      step() const { return m_step; }
    int         decimals() const { return m_decimals; }
    bool        zeroNull() const { return m_zeroNull; }
    
public slots:    
    void        setMinValue(double);
    void        setMaxValue(double);
    void        setStep(double);
    void        setDecimals(int);
    void        setZeroNull(bool);

private:
    double          m_minValue, m_maxValue, m_step;
    int             m_decimals;
    bool            m_zeroNull;
};
