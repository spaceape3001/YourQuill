////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

struct MetaDelegate::Repo {
    Vector<const MetaDelegate*>     all;
    Map<int,const MetaDelegate*>    byType;
};

MetaDelegate::Repo& MetaDelegate::repo()
{
    static Repo s_repo;
    return s_repo;
}

const Vector<const MetaDelegate*>&    MetaDelegate::all()
{
    return repo().all;
}



Delegate*                            MetaDelegate::create(int t, QObject* par)
{
    const MetaDelegate* md  = repo().byType.get(t, nullptr);
    if(!md) 
        return nullptr;
    return md -> create(par);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MetaDelegate::MetaDelegate(const char* k, const char*f) : MetaObject(k,f), m_qt{}
{
    Meta::m_type |= isDelegate;
    repo().all << this;
}

Delegate*       MetaDelegate::create() const
{
    return static_cast<Delegate*>(allocate());
}


Delegate*       MetaDelegate::create(QObject*par) const
{
    Delegate*   d   = create();
    if(par)
        d->setParent(par);
    return d;
}

void MetaDelegate::insert()
{
    MetaObject::insert();
    if(m_qt){
        repo().byType[m_qt] = this;
    }
}




//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;





Delegate::Delegate(QObject*parent) : QStyledItemDelegate(parent)
{
}

Delegate:: ~Delegate()
{
}

QVariant    Delegate::addDisplayVariant() const
{
    return tr("(add new)");
}

Compare        Delegate::compare(const QVariant&, const QVariant&) const
{
    return Compare::UNCOMPARABLE;
}

QWidget*    Delegate::createEditor(QWidget* parent) const
{
    return nullptr;
}

bool        Delegate::setEditorData(QWidget*, const QVariant&) const
{
    return false;
}

QVariant    Delegate::getEditorData(const QWidget*) const
{
    return QVariant();
}

bool        Delegate::paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const
{
    return false;
}

QVariant    Delegate::render(const QVariant&in) const
{
    return in;
}

QVariant    Delegate::render(const QVariant&in, const QModelIndex&idx) const
{
    return render(in);
}


//  ------------------------------------------------------------------------
//  These are the QItemDelegate ones
void        Delegate::updateEditorGeometry(
                QWidget*editor, 
                const QStyleOptionViewItem&option, 
                const QModelIndex& idx
            ) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, idx);
    //editor->setGeometry(option.rect);
}

QWidget*  Delegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, 
                    const QModelIndex & index ) const 
{
    QWidget* w  =  createEditor(parent);
    if(!w)
        w       = QStyledItemDelegate::createEditor(parent, option, index);
    return w;
}


void    Delegate::setEditorData (QWidget * editor, const QModelIndex & index ) const 
{
    if(!setEditorData(editor, index.data(Qt::EditRole))){
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void    Delegate::setModelData  (QWidget * editor, QAbstractItemModel * model, 
            const QModelIndex & index ) const 
{
    QVariant    data    = getEditorData(editor);
    if(data.isValid()){
        model->setData(index, data, Qt::EditRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void    Delegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, 
            const QModelIndex & index ) const 
{
    if(!paint(painter, option, index.data(Qt::DisplayRole))){
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool 	    Delegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) 
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

bool 	    Delegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) 
{
    return QStyledItemDelegate::helpEvent(event, view, option, index);
}

