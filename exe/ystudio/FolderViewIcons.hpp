////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QListView>
#include <QAbstractListModel>
#include <mithril/folder/Folder.hpp>

using yq::mithril::Folder;

class FolderViewIcons : public QListView {
    Q_OBJECT
public:

    class Model;
    
    FolderViewIcons(Folder, QWidget*parent=nullptr);
    FolderViewIcons(QWidget*parent=nullptr);
    ~FolderViewIcons();

    Folder          folder() const;

    Model*          model() { return m_model; }
    const Model*    model() const { return m_model; }

private:
    Model*      m_model = nullptr;
};

class FolderViewIcons::Model : public QAbstractListModel {
    Q_OBJECT
public:
    
    Model(Folder, QObject*parent=nullptr);
    ~Model();
    
    QVariant    data(const QModelIndex&, int) const override;
    //QModelIndex index(int,int,const QModelIndex&) const override;
    int         rowCount(const QModelIndex&) const override;
    
    Folder      folder() const { return m_folder; }
    
private:
    Folder      m_folder;
};
