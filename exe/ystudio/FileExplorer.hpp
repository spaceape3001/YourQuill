////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/SubWindow.hpp>
#include <vector>

using yq::gluon::SubWindow;
using yq::mithril::Folder;

class FileTree;
class QStackedLayout;
class FolderViewIcons;
class FolderViewDetails;
class QLineEdit;

class FileExplorer : public SubWindow {
    Q_OBJECT
public:
    FileExplorer(Folder, QWidget*parent=nullptr);
    FileExplorer(QWidget*parent=nullptr);
    ~FileExplorer();
    
private:
    FileTree*           m_tree      = nullptr;
    QStackedLayout*     m_stacked   = nullptr;
    FolderViewIcons*    m_byIcons   = nullptr;
    FolderViewDetails*  m_byDetails = nullptr;
    std::vector<Folder> m_history;
    size_t              m_hIndex    = 0;
    QLineEdit*          m_path      = nullptr;
};
