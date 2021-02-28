////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QDialog>
#include <util/TypeFwd.hpp>

class CheckBox;
class ComboBox;
class QFileDialog;

class WkspSelectorDlg : public QDialog {
    Q_OBJECT
public:
    WkspSelectorDlg(QWidget*parent=nullptr);
    virtual ~WkspSelectorDlg();
    
    void    addPathChoices(const QStringList&);
    void    setPath(const QString&);
    
    //bool    autoCreate() const;
    //void    setAutoCreate(bool);
    
    bool    autoLoad() const;
    void    setAutoLoad(bool);
    
    QString path() const;

private slots:
    void    browse();
    void    browseChanged(const QString&);
    
private:    
    ComboBox*       m_path;
    CheckBox*       m_autoLoad;
    //CheckBox*       m_create;
    QFileDialog*    m_fileDialog;
    QString         m_lastDir;
};
