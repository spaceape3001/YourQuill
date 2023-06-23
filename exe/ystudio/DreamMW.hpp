////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/MainWindow.hpp>

class Dock;

class DreamMW : public yq::gluon::MainWindow {
    Q_OBJECT
public:

    DreamMW();
    ~DreamMW();
    
    yq::gluon::MainWindow*   newMain() override;
    
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
    
private:
    std::vector<Dock*>  m_docks;
};
