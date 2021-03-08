////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/StdTableModel.hpp>
#include <db/AtomSys.hpp>
#include <dbgui/atomsys/ClassProvider.hpp>


class ClassTableModel : public StdTableModel<Class> {
    Q_OBJECT
public:
    
    ClassTableModel(ClassProvider ctp=ClassProvider(), QObject*parent=nullptr);
    ~ClassTableModel();

    //static Vector<QString>  availableColumns();

    void            append(const Class&) override;
    //int             column(const QString&) const;
    //int             columnCount(const QModelIndex&) const override;
    //QVariant        data(const QModelIndex&, int) const override;
    //size_t          dataSize() const;
    Class           getClass(int) const;
    Class           getClass(const QModelIndex& ) const;
    //Set<Class>      getTicked() const;
    //Qt::ItemFlags   flags(const QModelIndex&) const override;
    //QVariant        headerData(int, Qt::Orientation, int) const override;
    //bool            readOnly() const { return m_readOnly; }
    //int             rowCount(const QModelIndex&) const override;
    //bool            setData(const QModelIndex&, const QVariant&, int) override;
    
    //void            setColumns(std::initializer_list<QString>);
    //void            setReadOnly(bool);
    //void            setTickColumn(const QString&);
    //void            setTicked(const Set<Class>&);
    //void            sort(int column, Qt::SortOrder) override;

    /*! \brief Column to be the "tick"
    
        Column of -1 implies no tick
    */
    //int             tickColumn() const { return m_tickCol; }
    
public slots:
    void            refresh();
    void            check();

private:
    void            _refresh();
    //void            _allChanged();
    //void            _ticksChanged();

    
    ClassProvider           m_source;
    //Vector<CTMColumn>       m_columns;
    //Vector<CTMData>         m_data;
    Set<Class>              m_classes;
    //int                     m_tickCol;
    //bool                    m_readOnly;
};
