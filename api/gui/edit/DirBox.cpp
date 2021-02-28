////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirBox.hpp"
#include "LineEdit.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QToolButton>


DirBox::DirBox( QWidget*parent) : 
    QWidget(parent), m_edit(nullptr), m_button(nullptr)
{
    QHBoxLayout*        lay = new QHBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    m_edit              = new LineEdit;
    m_button    = new QToolButton;
    m_button -> setText("...");
    connect(m_button, &QToolButton::clicked, this, &DirBox::browse);
    connect(m_edit, &QLineEdit::textChanged, this, &DirBox::dirPathChanged);
    lay->addWidget(m_edit, 1);
    lay->addWidget(m_button, 0);
    setLayout(lay);
}

DirBox::~DirBox()
{
}

void     DirBox::browse()
{
    // TODO: Make this save/open sensitive, likely make it a member (non-static-method)
    QString     s = QFileDialog::getExistingDirectory(this, m_title, m_edit->text());
    if(!s.isEmpty())
        m_edit -> setText(s);
}

void     DirBox::clear()
{
    m_edit -> clear();
}

QString DirBox::dirPath() const
{
    return m_edit->text();
}


std::optional<QString>      DirBox::get() const
{
    return m_edit -> get();
}

void                        DirBox::set(const std::optional<QString>&v)
{
    m_edit -> set(v);
}


void    DirBox::setDirPath(const QString&f)
{
    m_edit->setText(f);
}

void    DirBox::setReadOnly(bool f)
{
    m_edit -> setReadOnly(f);
    m_button -> setEnabled(!f);
}


void    DirBox::setTitle(const QString&txt)
{
    m_title     = txt;
}


#include "moc_DirBox.cpp"
