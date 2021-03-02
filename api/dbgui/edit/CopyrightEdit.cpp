////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CopyrightEdit.hpp"

#include <db/Copyright.hpp>
#include <gui/edit/EnumCombo.hpp>
#include <gui/edit/LineEdit.hpp>
#include <gui/edit/SpinBox.hpp>

#include <QDate>
#include <QHBoxLayout>
#include <QLabel>

namespace {
    auto makeCopyrightValues() 
    {
        AssertDeny::QStringDescVec ret;
        ret.push_back({ QObject::tr("Copyright"), AssertDeny::Assert });
        ret.push_back({ QObject::tr("Disclaimer"), AssertDeny::Deny });
        ret.push_back({ QObject::tr("Notice"), AssertDeny::Neither });
        return ret;
    }
}


CopyrightEdit::CopyrightEdit(QWidget*parent) : QWidget(parent), m_stance(nullptr), m_from(nullptr), m_to(nullptr), m_text(nullptr)
{
    static int  year    = QDate::currentDate().year();
    
    QHBoxLayout*    lay = new QHBoxLayout(this);

    QLabel* dash    = new QLabel(tr("thru"), this);
    //dash -> setTextFormat(Qt::RichText);
    
    static const auto values    = makeCopyrightValues();

    m_stance  = new EnumCombo<AssertDeny>(values, this);
    m_from    = new SpinBox(this);
    m_from -> setRange(0, year);
    m_from -> setSpecialValueText(tr("ancient"));
    m_to      = new SpinBox(this);
    m_to -> setRange(0, year);
    m_to -> setSpecialValueText(tr("today"));
    m_text    = new LineEdit(this);
    
    lay -> addWidget(m_stance, 0);
    lay -> addWidget(m_from, 0);
    lay -> addWidget(dash, 0);
    lay -> addWidget(m_to, 0);
    lay -> addWidget(m_text, 1);
}

CopyrightEdit::~CopyrightEdit()
{
}

void    CopyrightEdit::clear()
{
    m_stance -> setValue(AssertDeny());
    m_from   -> reset();
    m_to     -> reset();
    m_text   -> clear();
}

unsigned int    CopyrightEdit::from() const
{
    return (unsigned int) m_from -> value();
}


Copyright       CopyrightEdit::get() const
{
    Copyright v;
    get(v);
    return v;
}

void    CopyrightEdit::get(Copyright&v) const
{
    v.text     = m_text -> text();
    v.stance   = m_stance -> value();
    v.from     = m_from -> value();
    v.to       = m_to -> value();
}

void    CopyrightEdit::set(const Copyright&v)
{
    m_text    -> setText(v.text.qString() );
    m_stance  -> set(v.stance);
    m_from    -> setValue(v.from);
    m_to      -> setValue(v.to);
}


void    CopyrightEdit::setFrom(unsigned int v)
{
    static unsigned int  year    = (unsigned int) QDate::currentDate().year();
    if(v <= year)
        m_from -> setValue((int) v);
}

void    CopyrightEdit::setReadOnly(bool f)
{
    m_stance  -> setEnabled(!f);
    m_from    -> setReadOnly(f);
    m_to      -> setReadOnly(f);
    m_text    -> setReadOnly(f);
}

void    CopyrightEdit::setText(const QString& v)
{
    m_text -> setText(v);
}

void    CopyrightEdit::setTo(unsigned int v)
{
    static unsigned int  year    = (unsigned int) QDate::currentDate().year();
    if(v <= year)
        m_to -> setValue((int) v);
}

void    CopyrightEdit::setUndoCallback(UndoCallback*undo)
{
    m_text    -> setUndoCallback(undo);
    m_stance  -> setUndoCallback(undo);
    m_from    -> setUndoCallback(undo);
    m_to      -> setUndoCallback(undo);
}

AssertDeny      CopyrightEdit::stance() const
{
    return m_stance -> value();
}

QString         CopyrightEdit::text() const
{
    return m_text -> text();
}

unsigned int    CopyrightEdit::to() const
{
    return (unsigned int) m_to -> value();
}

#include "moc_CopyrightEdit.cpp"
