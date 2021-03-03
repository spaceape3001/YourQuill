////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QWidget>
#include <db/AssertDeny.hpp>
#include <gui/undo/UndoBase.hpp>

struct Copyright;
template <typename> class EnumCombo;
class SpinBox;
class LineEdit;

class CopyrightEdit : public QWidget, public UndoBase {
    Q_OBJECT
public:

    static const AssertDeny::QStringDescVec&    comboValues();

    CopyrightEdit(QWidget*parent=nullptr);
    ~CopyrightEdit();
    
    unsigned int    from() const;
    Copyright       get() const;
    void            get(Copyright&) const;
    void            set(const Copyright&);
    void            setStance(AssertDeny);
    virtual void    setUndoCallback(UndoCallback*) override;
    AssertDeny      stance() const;
    unsigned int    to() const;
    QString         text() const;

public slots:
    void            setReadOnly(bool);
    void            clear();
    void            setFrom(unsigned int);
    void            setTo(unsigned int);
    void            setText(const QString&);
    
private:
    EnumCombo<AssertDeny>*  m_stance;
    SpinBox*                m_from;
    SpinBox*                m_to;
    LineEdit*               m_text;
};
