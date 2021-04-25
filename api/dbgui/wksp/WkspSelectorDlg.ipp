////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WkspSelectorDlg.hpp"

#include <db/Wksp.hpp>
#include <gui/edit/CheckBox.hpp>
#include <gui/edit/ComboBox.hpp>

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

WkspSelectorDlg::WkspSelectorDlg(QWidget*parent) : QDialog(parent), 
    m_fileDialog(nullptr)
{
    QDialogButtonBox*   btns    = new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Close);
    QPushButton*        brwse   = new QPushButton(tr("..."));
    connect(brwse, &QPushButton::clicked, this, &WkspSelectorDlg::browse);
    connect(btns, &QDialogButtonBox::accepted, this, &WkspSelectorDlg::accept);
    connect(btns, &QDialogButtonBox::rejected, this, &WkspSelectorDlg::reject);
    
    m_path              = new ComboBox;
    m_path->setEditable(true);
    m_autoLoad          = new CheckBox(tr("Automatically Load This Project in the Future."));
    //m_create            = new CheckBox(tr("Auto-create workspace if necessary"));
    
    QHBoxLayout*        hlay    = new QHBoxLayout;
    hlay->addWidget(m_path, 1);
    hlay->addWidget(brwse, 0);
    
    QVBoxLayout*        vlay    = new QVBoxLayout;
    vlay->addLayout(hlay);
    vlay->addWidget(m_autoLoad);
    //vlay->addWidget(m_create);
    vlay->addWidget(btns);
    setLayout(vlay);
    setWindowTitle("Select Workspace");
}

WkspSelectorDlg:: ~WkspSelectorDlg()
{
}


void    WkspSelectorDlg::addPathChoices(const QStringList& them)
{
    m_path->addItems(them);
}

//bool    WkspSelectorDlg::autoCreate() const
//{
    //return m_create -> isChecked();
//}

bool    WkspSelectorDlg::autoLoad() const
{
    return m_autoLoad->isChecked();
}

void     WkspSelectorDlg::browse()
{
    if(!m_fileDialog){
        m_fileDialog        = new QFileDialog(this, tr("Select Workspace"));
        m_fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        //m_fileDialog->setDefaultSuffix(szExtension());
        m_fileDialog->setFileMode(QFileDialog::Directory);
        m_fileDialog->setOption( QFileDialog::ShowDirsOnly, true);
        //m_fileDialog->setNameFilter(szFilter());
        connect(m_fileDialog, &QFileDialog::currentChanged, 
            this, &WkspSelectorDlg::browseChanged);
        
    }
    
    QString     txt     = m_path->currentText();
    if(!txt.isEmpty())
        m_fileDialog->selectFile(txt);
    m_fileDialog->show();
    if(m_fileDialog->exec() == QDialog::Accepted)
        m_path->setCurrentText( m_fileDialog->selectedFiles().first());
    m_fileDialog->hide();
}

void    WkspSelectorDlg::browseChanged(const QString& file)
{
    //static const QString  ewith = tr(".%1").arg(szExtension());
    if(m_fileDialog){
        if(QDir(file).exists(wksp::szQuillFile)){
            m_fileDialog->setLabelText(QFileDialog::Accept, tr("Open"));
        } else {
            m_fileDialog->setLabelText(QFileDialog::Accept, tr("Create"));
        }
    }
}


QString WkspSelectorDlg::path() const
{
    return m_path->currentText();
}

//void    WkspSelectorDlg::setAutoCreate(bool f)
//{
    //m_create->setChecked(f);
//}


void    WkspSelectorDlg::setAutoLoad(bool f)
{
    m_autoLoad->setChecked(f);
}


void    WkspSelectorDlg::setPath(const QString&s)
{
    m_path->setCurrentText(s);
    
}

