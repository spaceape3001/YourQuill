////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SimpleEditor.hpp"
//#include "SubWin.hpp"
//#include "UndoMain.hpp"
//#include "UndoWin.hpp"
//#include "UndoBase.hpp"
//#include "TabBar.hpp"
//#include "TabWidget.hpp"

//#include "core/Logging.hpp"
//#include "core/Utilities.hpp"
////#include "math/MathUtils.hpp"

//#include <QAction>
//#include <QCloseEvent>
//#include <QDockWidget>
//#include <QDrag>
//#include <QDragEnterEvent>
//#include <QDragMoveEvent>
//#include <QDropEvent>
#include <QFileDialog>
//#include <QIcon>
//#include <QKeySequence>
//#include <QMdiArea>
//#include <QMdiSubWindow>
//#include <QMenu>
//#include <QMenuBar>
#include <QMessageBox>
//#include <QMetaMethod>
//#include <QMetaObject>
//#include <QMimeData>
//#include <QMouseEvent>
//#include <QPixmap>
//#include <QRect>
//#include <QRegion>
//#include <QSignalMapper>
//#include <QStatusBar>
//#include <QTimer>
//#include <QUndoStack>


SimpleEditor::SimpleEditor(QWidget* editor) : UndoMain(editor)
{
    addAction("new", tr("New")).shortcut(tr("Ctrl+N")).connect(this, &SimpleEditor::cmdNew);
    addAction("open", tr("Open")).shortcut(tr("Ctrl+O")).connect(this, &SimpleEditor::cmdOpen);
    addAction("save", tr("Save")).shortcut(tr("Ctrl+S")).connect(this, &SimpleEditor::cmdSave);
    addAction("saveas", tr("Save As")).shortcut(tr("Ctrl+Shift+S")).connect(this, &SimpleEditor::cmdSaveAs);

    addToMenu(makeMenu("file","&File"), QStringList() 
        << "new"
        << "open"
        << "--"
        << "save"
        << "saveas"
        //<< "--"
        //<< "exit"
    );
    
    addToMenu(makeMenu("edit", "&Edit"), QStringList()
        << "undo"
        << "redo"
    );


    resize(1920, 1080);
    status(tr("Welcome"));

}

SimpleEditor::~SimpleEditor()
{
}


void        SimpleEditor::cmdNew()
{
    if(okayToClose()){
                //  Destroy any possible pointers into the editor's data 
                //  space
        clearUndo();
        newFile();
        m_file.clear();
        setClean();
        updateTitle();
    }
}

void        SimpleEditor::cmdOpen()
{
    if(okayToClose()){
        QString     newFile = promptOpenFile();
        if(newFile.isEmpty())
            return ;
        if(openFile(newFile)){
            clearUndo();
            m_file  = newFile;
            setClean();
            updateTitle();
        }
    }
}

void        SimpleEditor::cmdSave()
{
    if(m_file.isEmpty()){
        cmdSaveAs();
    } else {
        if(saveFile(m_file.full())){
            setClean();
            updateTitle();
        } else {
            QMessageBox::critical(this, tr("Save FAILED"), 
                tr("Unable to save to %1").arg(m_file.full()));
        }
    }
}

void        SimpleEditor::cmdSaveAs()
{
    QString     newName = promptSaveFile();
    if(!newName.isEmpty()){
        if(newName == m_file.full()){
            cmdSave();
        } else if(saveFile(newName)){
            m_file  = newName;
            setClean();
            updateTitle();
        } else {
            QMessageBox::critical(this, tr("Save FAILED"), 
                tr("Unable to save to %1").arg(newName));
        }
    }
}


bool        SimpleEditor::okayToClose()
{
    if(isClean())
        return true;
        
    auto sb = QMessageBox::warning(this, tr("Save %1?").arg(ourData()),
        tr("File \"%1\" has been modified, save?").arg(m_file.full()),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
    switch(sb){
    case QMessageBox::Save:
        cmdSave();
        return isClean();
    case QMessageBox::Discard:
        return true;
    case QMessageBox::Cancel:
    default:
        return false;
    }
}

QString     SimpleEditor::promptOpenFile()
{
    return QFileDialog::getOpenFileName(this, tr("Open %1...").arg(ourData()), m_file.path(), ourSaveType());
}

QString     SimpleEditor::promptSaveFile()
{
    return QFileDialog::getSaveFileName(this, tr("Save %1 to...").arg(ourData()), m_file.path(), ourSaveType());
}



void        SimpleEditor::updateTitle()
{
    QString     title;
    if(!isClean())
        title += "*";
    if(m_file.isEmpty()){
        title += "(untitled)";
    } else 
        title += m_file.name() + " - " + m_file.path();
    title += " - " + ourName();
    setWindowTitle(title);
}

#include "moc_SimpleEditor.cpp"
