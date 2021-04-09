////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/main/SubWin.hpp>
#include <db/FileSys.hpp>
#include <dbgui/document/DocumentQ.hpp>
#include <dbgui/folder/FolderQ.hpp>

class QAction;
class QComboBox;
class QStackedWidget;
class QToolBar;
class QUndoStack;
class QVBoxLayout;

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
    
    bool            isShowHiddenEnabled() const { return m_showHidden; }
    
    void            cmdNavigateTo(Folder);

signals:
    void            folderChanged();
    void            openRequest(Document);
    
public slots:
    void            cmdGoBack();
    void            cmdGoForward();
    void            cmdGoUp();
    void            cmdReload();
    void            setShowHidden(bool);

public slots:
    void            refresh();

protected:

    void        addLeft(QWidget*, int stretch=0);
    void        addRight(QWidget*, int stretch=0);
    void        addNavBar(QWidget*);
    void        addNavSeparator();
    void        showTable();
    void        showList();
    
private:

    class ChgFolder;

    //  TODO Want to add middle button & context menu support

    QAction*        m_backAct       = nullptr;
    QAction*        m_forwardAct    = nullptr;
    QAction*        m_upAct         = nullptr;

    Folder          m_folder        = Folder{};
    QUndoStack*     m_history       = nullptr;
    
    ListModel*      m_listModel     = nullptr;
    int             m_listPos       = -1;
    ListView*       m_listView      = nullptr;
    QStackedWidget* m_stacked       = nullptr;
    TableModel*     m_tableModel    = nullptr;
    int             m_tablePos      = -1;
    TableView*      m_tableView     = nullptr;
    TreeModel*      m_treeModel     = nullptr;
    TreeView*       m_treeView      = nullptr;
    QComboBox*      m_url           = nullptr;
    bool            m_showHidden    = false;
    QVBoxLayout*    m_left          = nullptr;
    QVBoxLayout*    m_right         = nullptr;
    QToolBar*       m_navbar        = nullptr;
    
    void            changeTo(Folder);
    
    static Vector<Entry>    makeEntries(const Vector<Document>&);
    static Vector<Entry>    makeEntries(const Vector<Folder>&);
    static Vector<Entry>    makeEntries(const Vector<cdb::DocOrFold>&);
    
    
    unsigned        searchOpts() const;
   
   enum class Type {
        None    = 0,
        Doc,
//        Dir,
        Fold
//        Frag
    };
 
};
