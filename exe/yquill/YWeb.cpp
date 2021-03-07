////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YWeb.hpp"

YWeb::YWeb(QWidget*parent) : WebBase(parent)
{
    connect(this, &YWeb::titleChanged, this, &YWeb::update_title);
    goHome();
}

YWeb::~YWeb()
{
}

void    YWeb::update_title()
{
    QString     t   = title();
    if(t.isEmpty())
        setWindowTitle("Web");
    else
        setWindowTitle(tr("%1 - Web").arg(t));
}


#include "moc_YWeb.cpp"
