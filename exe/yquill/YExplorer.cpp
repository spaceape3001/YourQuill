////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YExplorer.hpp"

YExplorer::YExplorer(QWidget* parent)
{
    cmdNavigateTo(cdb::top_folder());
    updateTitle();
    connect(this, &FolderExplorerBase::folderChanged, this, &YExplorer::updateTitle);
}

YExplorer::~YExplorer()
{
}

void        YExplorer::updateTitle()
{
    QString     k   = cdb::key(folder());
    if(!k.isEmpty())
        setWindowTitle(tr("Explore [%1]").arg(k));
    else
        setWindowTitle(tr("Explore"));
}


#include "moc_YExplorer.cpp"

