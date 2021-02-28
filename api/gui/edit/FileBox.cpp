////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileBox.hpp"
#include "LineEdit.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QToolButton>


FileBox::FileBox( QWidget*parent) : 
    QWidget(parent), m_edit(nullptr), m_button(nullptr)
{
    QHBoxLayout*        lay = new QHBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    m_edit              = new LineEdit;
    m_button            = new QToolButton;
    m_button -> setText("...");
    connect(m_button, &QToolButton::clicked, this, &FileBox::browse);
    connect(m_edit, &QLineEdit::textChanged, this, &FileBox::fileChanged);
    lay->addWidget(m_edit, 1);
    lay->addWidget(m_button, 0);
    setLayout(lay);
}

FileBox::~FileBox()
{
}

void     FileBox::browse()
{
    // TODO: Make this save/open sensitive, likely make it a member (non-static-method)
    QString     s = QFileDialog::getOpenFileName(this, m_title, QString(), m_filter);
    if(!s.isEmpty())
        m_edit -> setText(s);
}

void    FileBox::clear()
{
    m_edit -> clear();
}


QString FileBox::file() const
{
    return m_edit->text();
}

std::optional<QString>      FileBox::get() const
{
    return m_edit -> get();
}

void                        FileBox::set(const std::optional<QString>&v)
{
    m_edit -> set(v);
}


void    FileBox::setFile(const QString&f)
{
    m_edit->setText(f);
}

void    FileBox::setFilter(const QString&txt)
{
    m_filter    = txt;
}

void    FileBox::setReadOnly(bool b)
{
    m_edit -> setReadOnly(b);
    m_button -> setEnabled(!b);
}


void    FileBox::setTitle(const QString&txt)
{
    m_title     = txt;
}



#include "moc_FileBox.cpp"
