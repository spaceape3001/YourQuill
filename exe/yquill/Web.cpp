////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Web.hpp"

Web::Web(QWidget*parent) : WebBase(parent)
{
    connect(this, &Web::titleChanged, this, &Web::update_title);
    goHome();
}

Web::~Web()
{
}

void    Web::update_title()
{
    QString     t   = title();
    if(t.isEmpty())
        setWindowTitle("Web");
    else
        setWindowTitle(tr("%1 - Web").arg(t));
}


#include "moc_Web.cpp"
