////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QWksp.hpp"

#include <db/QuillFile.hpp>
#include <db/Workspace.hpp>
#include <dbgui/quill/QuillEdit.hpp>
#include <util/Utilities.hpp>

#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>

QWksp::QWksp() : SimpleEditor(new QuillEdit), m_editor(nullptr)
{
    m_editor    = static_cast<QuillEdit*>(editor());
    connect(m_editor, &QuillEdit::cmdAddRootDir, this, &QWksp::cmdAddChain);
    connect(m_editor, &QuillEdit::cmdAddTemplate, this, &QWksp::cmdAddTemplate);
    //connect(m_editor, &WkspEditor::remReq, this, &QWksp::cmdRemChain);
    updateTitle();
}

QWksp::~QWksp()
{
}

bool        QWksp::newFile() 
{
    m_editor -> clear();
    return true;
}

bool        QWksp::openFile(const QString&fname) 
{
    QuillFile data;
    if(data.load(QDir(fname).absoluteFilePath(".yquill"))){
        m_editor -> clear();
        m_editor -> set(data);
        return true;
    } else 
        return false;
}

bool        QWksp::saveFile(const QString&fname) const 
{
    QuillFile  data;
    m_editor -> get(data);
    return data.save_to(QDir(fname).absoluteFilePath(".yquill"));
}


void        QWksp::cmdAddChain()
{
    QString newDir = QFileDialog::getExistingDirectory(this, tr("Select Quill Workspace"), 
        file().path(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(newDir.isEmpty())
        return ;
    m_editor -> addRoot(newDir);
}

void        QWksp::cmdAddTemplate()
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
QString     QWksp::promptOpenFile()
{
    return QFileDialog::getExistingDirectory(this, tr("Open Quill Workspace"), 
        file().path(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
}


//  Override to alter save-file-dialog
QString     QWksp::promptSaveFile()
{
    return QFileDialog::getExistingDirectory(this, tr("Save Quill Workspace"), 
        file().path(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


int main(int argc, char* argv[])
{
    QApplication    app(argc, argv);
    QWksp      mw;
    mw.show();
    return app.exec();
}

#include "moc_QWksp.cpp"

