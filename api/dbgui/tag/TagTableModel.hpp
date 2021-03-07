////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/Tag.hpp>
#include <dbgui/tag/TagProvider.hpp>
#include <gui/model/StdTableModel.hpp>
#include <util/Set.hpp>

class TagTableModel : public StdTableModel<Tag> {
    Q_OBJECT
public:

    TagTableModel(TagProvider stp, QObject*parent=nullptr);
    ~TagTableModel();
    
    void            append(const Tag&) override;
    //int             columnCount(const QModelIndex&) const override;
    //QVariant        data(const QModelIndex&, int) const override;
    //size_t          dataSize() const;
    //QVariant        headerData(int, Qt::Orientation, int) const override;
    //Qt::ItemFlags   flags(const QModelIndex&) const override;
    //bool            readOnly() const;
    //int             rowCount(const QModelIndex&) const override;
    //bool            setData(const QModelIndex&, const QVariant&, int) override;
    //void            setReadOnly(bool);
    Tag             tag(int) const;
    Tag             tag(const QModelIndex&) const;
    
public slots:
    void            refresh();
    void            check();

private:    
    void            _refresh();
    
    TagProvider     m_source;
    Set<Tag>        m_tags;     // from the last pull
};

