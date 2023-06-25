////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/MainWindow.hpp>
#include <basic/Hash.hpp>
#include "ystudio.hpp"

using MainWindow            = yq::gluon::MainWindow;

class DreamMW : public MainWindow {
    Q_OBJECT
public:

    DreamMW();
    ~DreamMW();
    
    MainWindow*   newMain() override;
    
public slots:
    void    updateTitle();
    
    void    newAtomTable();
    void    newBrowser();
    void    newBookTable();
    void    newCategoryTable();
    void    newCharacterTable();
    void    newClassTable();
    void    newDirectoryTable();
    void    newDocumentTable();
    void    newEventTable();
    void    newFieldTable();
    void    newFolderTable();
    void    newFragmentTable();
    void    newGameTable();
    void    newLeafTable();
    void    newPlaceTable();
    void    newRootTable();
    void    newTagList();
    void    newTagTable();
    
private slots:
    void    popupRequested(Id);
    void    openRequested(Id);
    void    commandTriggered(uint64_t);
    
protected:
    virtual QDockWidget*    addDock(Qt::DockWidgetArea, QWidget*) override;
    virtual void            reconnect(QWidget*) override;    

private:
    std::vector<Dock*>                      m_docks;
    std::unordered_map<uint64_t, QAction*>  m_cmdActions;
    Id                                      m_idForCmd  = Id();
};
