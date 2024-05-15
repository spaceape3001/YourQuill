////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DoodleMW.hpp"


DoodleMW::DoodleMW(yq::doodler::QProjectPtr qpp) : m_project(qpp)
{
    using namespace yq;
    using namespace yq::doodler;
    if(!m_project)
        m_project   = new QProject;
        
    addAction("clear", "Clear")
        .icon(QIcon(":/icon/clear.svg"))
        .shortcut(QKeySequence::Delete)
        .connect(this, &DoodleMW::cmdClear)
    ;
    addAction("copy", "Copy")
        .icon(QIcon(":/icon/copy.svg"))
        .shortcut(QKeySequence::Copy)
        .connect(this, &DoodleMW::cmdCopy)
    ;
    addAction("cut", "Cut")
        .icon(QIcon(":/icon/cut.svg"))
        .shortcut(QKeySequence::Cut)
        .connect(this, &DoodleMW::cmdCut)
    ;
    addAction("find", "Find")
        .icon(QIcon(":/icon/find.svg"))
        .shortcut(QKeySequence::Find)
        .connect(this, &DoodleMW::cmdFind)
    ;
    addAction("paste", "Paste")
        .icon(QIcon(":/icon/paste.svg"))
        .shortcut(QKeySequence::Paste)
        .connect(this, &DoodleMW::cmdPaste)
    ;
    addAction("print", "Print")
        .icon(QIcon(":/icon/print.svg"))
        .shortcut(QKeySequence::Print)
        .connect(this, &DoodleMW::cmdPrint)
    ;
    addAction("open", "Open")
        .icon(QIcon(":/icon/open.svg"))
        .shortcut(QKeySequence::Open)
        .connect(this, &DoodleMW::cmdOpen)
    ;
    addAction("redo", "Redo")
        .icon(QIcon(":/icon/redo.svg"))
        .shortcut(QKeySequence::Redo)
    ;
    addAction("refresh", "Refresh")
        .icon(QIcon(":/icon/refresh.svg"))
        .shortcut(QKeySequence::Refresh)
    ;
    
    addAction("save", "Save")
        .icon(QIcon(":/icon/save.svg"))
        .shortcut(QKeySequence::Save)
        .connect(this, &DoodleMW::cmdSave)
    ;
    
    addAction("saveas", "Save As")
        .icon(QIcon(":/icon/saveas.svg"))
        .shortcut(QKeySequence::SaveAs)
        .connect(this, &DoodleMW::cmdSave)
    ;

    addAction("undo", "Undo").icon(QIcon(":/icon/undo.svg")).shortcut(QKeySequence::Undo);
    addAction("zoomIn", "Zoom In").icon(QIcon(":/icon/zoomIn.svg")).shortcut(QKeySequence::ZoomIn);
    addAction("zoomOut", "Zoom In").icon(QIcon(":/icon/zoomOut.svg")).shortcut(QKeySequence::ZoomOut);


    QMenu* fileMenu   = makeMenu("file", "&File");
    QMenu* editMenu   = makeMenu("edit", "&Edit");
    QMenu* viewMenu   = makeMenu("view", "&View");
    QMenu* doodleMenu = makeMenu("doodle", "&Doodle");

    addToMenu(fileMenu,
        QStringList()
            << "--"
            << "open"
            << "save"
            << "saveas"
    );

    addToMenu(editMenu,
        QStringList()
            << "undo"
            << "redo"
            << "--"
            << "cut"
            << "copy"
            << "paste"
            << "clear"
            << "--"
            << "find"
    );
    
    addToMenu(viewMenu,
        QStringList() 
            << "zoomIn"
            << "zoomOut"
    );
        
    enableDetachableTabs();
    resize(1920, 1080);
    updateTitle();
    
    connect(m_project.ptr(), &QProject::fileChanged, this, &DoodleMW::updateTitle);
    connect(m_project.ptr(), &QProject::titleChanged, this, &DoodleMW::updateTitle);
}

DoodleMW::~DoodleMW()
{
}

void      DoodleMW::cmdClear()
{
}

void      DoodleMW::cmdCopy()
{
}

void      DoodleMW::cmdCut()
{
}

void      DoodleMW::cmdFind()
{
}

void      DoodleMW::cmdOpen()
{
}

void      DoodleMW::cmdPaste()
{
}

void      DoodleMW::cmdPrint()
{
}

void      DoodleMW::cmdRedo()
{
}

void      DoodleMW::cmdSave()
{
}

void      DoodleMW::cmdSaveAs()
{
}

void      DoodleMW::cmdSetTitle()
{
}


void      DoodleMW::cmdUndo()
{
}


DoodleMW* DoodleMW::newMain()
{
    return new DoodleMW(m_project);
}

void      DoodleMW::updateTitle()
{
    setWindowTitle(
        tr("Doodler (%1)[%2]")
            .arg(m_project->title())
            .arg(m_project->fileName())
    );
}


#include "moc_DoodleMW.cpp"

