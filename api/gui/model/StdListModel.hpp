////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractListModel>

template <typename> class StdAdder;
template <typename> class StdColumn;
template <typename> class StdColumnWriter;
template <typename> class StdDelegator;

template <typename T>
class StdListModel : public QAbstractListModel {
public:

    virtual void        append(const T&);
    virtual void        append(const Vector<T>&);

    virtual void        clear();
    QVariant            data(const QModelIndex&, int) const override;
    Qt::ItemFlags       flags(const QModelIndex&) const override;
    QVariant            headerData(int, Qt::Orientation, int) const override;
    virtual void        moveRowUp(int);
    virtual void        moveRowDown(int);
    bool                readOnly() const { return m_readOnly; }

    template <typename Pred>
    void                removeRowIf(Pred);
    
    virtual void        removeRow(int);

    int                 rowCount() const;
    int                 rowCount(const QModelIndex&) const override { return rowCount(); }

    bool                setData(const QModelIndex&, const QVariant&, int) override;
    void                setReadOnly(bool);
    void                setTickable(bool);
     
    void                sort(int, Qt::SortOrder) override;
    bool                tickable() const { return m_tickable; }
    
    using Column        = StdColumn<T>;
    using Adder         = StdAdder<T>;
    using Delegator     = StdDelegator<T>;
    using ColumnWriter  = StdColumnWriter<T>;

    struct Row {
        T               data;
        bool            tick;
    };
    
    const Vector<Row>&      rows() const { return m_rows; }
    
    const Column*       column() const { return m_column; }
    
protected:
    Vector<Row>         m_rows;
    StdColumn<T>*       m_column;
    StdColumn<T>*       m_header;
    QVariant            m_label;
    bool                m_adder;
    bool                m_readOnly;
    bool                m_canAdd;
    bool                m_dragReorder;  // pending
    bool                m_tickable;

    StdListModel(QObject* parent=nullptr);
    virtual ~StdListModel();

    Row*                _row(int r);
    const Row*          _row(int r) const;

    //! Returns the pointer into the vector for the given index (or NULL if not good)
    Row*                _row(const QModelIndex& idx);
    //! Returns the pointer into the vector for the given index (or NULL if not good)
    const Row*          _row(const QModelIndex& idx) const;
    
    void                allChanged();

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
};
