////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractTableModel>

#include <meta/Variant.hpp>
#include <util/Compare.hpp>
#include <util/EnumMapFwd.hpp>
#include <util/Vector.hpp>

#include <QVariant>
#include <exception>

class QValidator;

template <typename> class StdAdder;
template <typename> class StdColumn;
template <typename> class StdColumnWriter;
template <typename> class StdDelegator;

template <typename T>
class StdTableModel : public QAbstractTableModel {
public:

    using Base = StdTableModel;

    StdTableModel(QObject*parent=nullptr);
    virtual ~StdTableModel();
    
    virtual void        append(const T&);
    virtual void        append(const Vector<T>&);

    virtual void        clear();
    int                 column(const String&) const;

    int                 columnCount() const
    {
        return (int) m_columns.size();
    }
    virtual int         columnCount(const QModelIndex&) const override 
    { 
        return columnCount();
    }
    
    String              columnKey(int) const;
    String              columnKey(const QModelIndex&) const;
    
    virtual QVariant    data(const QModelIndex&, int) const override;
    
    Qt::ItemFlags       flags(const QModelIndex&) const override;
    QVariant            headerData(int, Qt::Orientation, int) const override;
    
    virtual void        moveRowUp(int);
    virtual void        moveRowDown(int);
    
    
    bool                readOnly() const { return m_readOnly; }
    
    template <typename Pred>
    void                removeRowIf(Pred);
    
    virtual void        removeRow(int);
    
    T*                  row(int);
    const T*            row(int) const;
    T*                  row(const QModelIndex&);
    const T*            row(const QModelIndex&) const;

    int                 rowCount() const;
    
    virtual int         rowCount(const QModelIndex&) const override
    {
        return rowCount();
    }
    
    void                setAllData(const std::vector<T>&);

    bool                setData(const QModelIndex&, const QVariant&, int) override;
    void                setReadOnly(bool);
    void                setTickColumn(const String&);
    void                setTickColumn(int);
     
    void                sort(int, Qt::SortOrder) override;
    
    /*! \brief Column to be the "tick"
        \note -1 implies no tick (default)
    */
    int                 tickColumn() const { return m_tickCol; }
    
    void                updateTickColumn();
    
    using Column        = StdColumn<T>;
    using Adder         = StdAdder<T>;
    using Delegator     = StdDelegator<T>;
    using ColumnWriter  = StdColumnWriter<T>;
    
    struct Row {
        T               data;
        bool            tick;
    };
    
    const Vector<Column*>&  columns() const { return m_columns; }
    const Vector<Row>&      rows() const { return m_rows; }


    //void                hideColumn

    ColumnWriter        col(Column* c);
    ColumnWriter        col(const String&, String(T::*), bool cs=false);
    template <typename E>
    ColumnWriter        col(const String&, EnumImpl<E> (T::*));
    template <typename K, typename V>
    ColumnWriter        col(EnumMap<K,EnumImpl<V>> (T::*), K);
    template <typename K, typename V>
    ColumnWriter        col(const String&, EnumMap<K,EnumImpl<V>> (T::*), K);

    //! Creates a custom read-only column using lambdas
    template <typename Getter>
    ColumnWriter        customRO(const String&, Getter);
    
    //! Creates a custom read-only column using lambdas
    template <typename Getter, typename Less>
    ColumnWriter        customRO(const String&, Getter, Less);
    
    //! Creates a custom read-write column using lambdas
    template <typename Getter, typename Setter>
    ColumnWriter        customRW(const String&, Getter, Setter);
    
    //! Creates a custom read-write column using lambdas
    template <typename Getter, typename Setter, typename Less>
    ColumnWriter        customRW(const String&, Getter, Setter, Less);

protected:

    Vector<Column*>     m_columns;
    Vector<Row>         m_rows;
    size_t              m_adders        = 0;
    bool                m_readOnly      = false;
    bool                m_canAdd        = false;
    bool                m_dragReorder   = false;  // pending...
    int                 m_tickCol       = -1;
    
    void                allChanged();
    

    //template <String (T::*)>
    //void                add(const String&, const QVariant&v=QVariant, Adder* a=nullptr)
    

    const Column*       _column(int c) const;
    const Column*       _column(const QModelIndex& idx) const;

    Row*                _row(int r);
    const Row*          _row(int r) const;

    //! Returns the pointer into the vector for the given index (or NULL if not good)
    Row*                _row(const QModelIndex& idx);
    //! Returns the pointer into the vector for the given index (or NULL if not good)
    const Row*          _row(const QModelIndex& idx) const;
    
};
