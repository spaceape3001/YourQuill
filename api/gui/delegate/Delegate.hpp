////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QStyledItemDelegate>
#include <meta/Object.hpp>
#include <util/Compare.hpp>

class MetaValue;
class MetaDelegate;
template <typename> class TypedMetaDelegate;
class Delegate;

class MetaDelegate : public MetaObject {
    template <typename> friend class TypedMetaDelegate;
public:

    static const Vector<const MetaDelegate*>&   all();
    static Delegate*                            create(int, QObject* par=nullptr);

    Delegate*           create() const;
    Delegate*           create(QObject*par) const;

    int                 qt() const { return m_qt; }
    //const MetaValue&    value() const { return *m_value; }
    
protected:
    MetaDelegate(const char*, const char*);
    virtual void insert() override;
    
private:
    //const MetaValue*    m_value;
    int                 m_qt;
    
    struct Repo;
    static Repo&    repo();
    static void     freeze();
};

/*! \brief Generic delegate to wrapper the idiosyncracies of the Qt's delegate
*/
class Delegate : public QStyledItemDelegate, public Object {
    Q_OBJECT
    MO_META(MetaDelegate)
    MO_TYPED(TypedMetaDelegate)
    MO_DECLARE(Delegate, Object)
public:

    using DataType  = void;

    class Info;
    template <typename D>  class TypedInfo;
    
    static const Vector<const Info*>&   allInfo();
    
    static Delegate*    make(int dataType, QObject* parent=nullptr);
    
    //  ------------------------------------------------------------------------
    //  These are what the derived class are expected to extend....
    
    
    //! Returns what to display in case it's an (add) situation
    virtual QVariant    addDisplayVariant() const;

    //! Less than comparison (same as a sort, etc)
    virtual Compare     compare(const QVariant&, const QVariant&) const; 
    virtual QWidget*    createEditor(QWidget* parent=nullptr) const;
    virtual bool        setEditorData(QWidget*, const QVariant&) const;
    virtual QVariant    getEditorData(const QWidget*) const;
    virtual bool        paint(QPainter*, const QStyleOptionViewItem& option, const QVariant&) const;
    
    //  In case there are any needed formatting (to string)
    virtual QVariant    render(const QVariant&) const;
    virtual QVariant    render(const QVariant&, const QModelIndex&) const;
    
    //virtual QString 	displayText(const QVariant &value, const QLocale &locale) const;    
    
    //  Yes, this means using the older macros....
    virtual void        connectEditedSignal(QWidget*, QObject*, const char*) const {}
    
    //virtual QSize       sizeHint(const QStyleOptionViewItem&) const;

    //  ------------------------------------------------------------------------
    //  These are the QItemDelegate ones
    virtual QWidget*  createEditor ( QWidget * parent, const QStyleOptionViewItem & option, 
                        const QModelIndex & index ) const override;

    virtual void        setEditorData (QWidget * editor, const QModelIndex & index ) const override;
    virtual void        setModelData  (QWidget * editor, QAbstractItemModel * model, 
                            const QModelIndex & index ) const override;
    virtual void        updateEditorGeometry ( QWidget * editor, 
                            const QStyleOptionViewItem & option, 
                            const QModelIndex & index ) const override;
    virtual void        paint ( QPainter * painter, const QStyleOptionViewItem & option, 
                            const QModelIndex & index ) const override;
    virtual bool 	    editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    virtual bool 	    helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    
protected:
    Delegate(QObject* parent=nullptr);
    virtual ~Delegate();
};


