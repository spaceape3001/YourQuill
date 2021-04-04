////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <db/FileSys.hpp>
#include <util/Stack.hpp>

class QStackedWidget;
class QComboBox;

class FolderExplorerBase : public SubWin {
    Q_OBJECT
public:

    FolderExplorerBase(QWidget*parent=nullptr);
    ~FolderExplorerBase();
  
    Folder          folder() const { return m_folder; }
  
    struct Entry;
    class ListModel;
    class ListView;
    class TableModel;
    class TableView;
    class TreeModel;
    class TreeView;
    struct NameCol;
    
private slots:
    void            cmdGoBack();
    void            cmdGoForward();
    void            cmdGoUp();
  
private:

    Folder          m_folder;
    Stack<Folder>   m_history;
    ListModel*      m_listModel;
    int             m_listPos;
    ListView*       m_listView;
    QStackedWidget* m_stacked;
    TableModel*     m_tableModel;
    int             m_tablePos;
    TableView*      m_tableView;
    TreeModel*      m_treeModel;
    TreeView*       m_treeView;
    QComboBox*      m_url;
    
    void            changeTo(Folder);
   
   enum class Type {
        None    = 0,
        Doc,
        Dir,
        Fold,
        Frag
    };
 
};
