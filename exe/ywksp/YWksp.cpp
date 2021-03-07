////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "YWksp.hpp"

#include <db/QuillFile.hpp>
#include <db/Workspace.hpp>
#include <dbgui/quill/QuillEdit.hpp>
#include <util/Utilities.hpp>

#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>

YWksp::YWksp() : SimpleEditor(new QuillEdit), m_editor(nullptr)
{
    m_editor    = static_cast<QuillEdit*>(editor());
    connect(m_editor, &QuillEdit::cmdAddRootDir, this, &YWksp::cmdAddChain);
    connect(m_editor, &QuillEdit::cmdAddTemplate, this, &YWksp::cmdAddTemplate);
    //connect(m_editor, &WkspEditor::remReq, this, &YWksp::cmdRemChain);
    updateTitle();
}

YWksp::~YWksp()
{
}

bool        YWksp::newFile() 
{
    m_editor -> clear();
    return true;
}

bool        YWksp::openFile(const QString&fname) 
{
    QuillFile data;
    if(data.load(QDir(fname).absoluteFilePath(".yquill"))){
        m_editor -> clear();
        m_editor -> set(data);
        return true;
    } else 
        return false;
}

bool        YWksp::saveFile(const QString&fname) const 
{
    QuillFile  data;
    m_editor -> get(data);
    return data.save_to(QDir(fname).absoluteFilePath(".yquill"));
}


void        YWksp::cmdAddChain()
{
    QString newDir = QFileDialog::getExistingDirectory(this, tr("Select Quill Workspace"), 
        file().path(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(newDir.isEmpty())
        return ;
    m_editor -> addRoot(newDir);
}

void        YWksp::cmdAddTemplate()
{
    QStringSet       avail   = wksp::templates_available();
    avail -= m_editor -> templates();
    if(avail.empty())
        return;
    bool    ok  = false;
    QString     txt = QInputDialog::getItem(this, tr("Chooser"), tr("Select template to add"), qt_list(avail), 0, false, &ok);
    if(ok && !txt.isEmpty())
        m_editor -> addTemplate(txt);
}


//  Override to alter open-file-dialog
QString     YWksp::promptOpenFile()
{
    return QFileDialog::getExistingDirectory(this, tr("Open Quill Workspace"), 
        file().path(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
}


//  Override to alter save-file-dialog
QString     YWksp::promptSaveFile()
{
    return QFileDialog::getExistingDirectory(this, tr("Save Quill Workspace"), 
        file().path(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


int main(int argc, char* argv[])
{
    QApplication    app(argc, argv);
    YWksp      mw;
    mw.show();
    return app.exec();
}

#include "moc_YWksp.cpp"

