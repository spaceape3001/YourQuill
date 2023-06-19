////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"
#include "IdListModel.hpp"
#include "IdTableModel.hpp"
#include "IdColumn.hpp"

namespace yq::mithril {

    IdModel::IdModel(Type t, Id i, IdProvider&& p, QObject*parent) : 
        QAbstractItemModel(parent), m_type(t), m_root(this, i, nullptr, std::move(p))
    {
    }
    
    IdModel::~IdModel()
    {
    }
    
    void    IdModel::_load()
    {
        m_root.reload();
    }

    void    IdModel::addColumn(IdColumn&& col)
    {
        col.number  = m_columns.size();
        m_columns.push_back(std::move(col));
    }
    
    void    IdModel::addColumn(size_t before, IdColumn&& col)
    {
        if(before < m_columns.size()){
            m_columns.insert(m_columns.begin()+before, std::move(col));
            for(size_t n=before; n<m_columns.size(); ++n)
                m_columns[n].number    = n;
        } else {
            col.number  = m_columns.size();
            m_columns.push_back(std::move(col));
        }
    }

    const IdColumn* IdModel::column(size_t n) const
    {
        if(n<m_columns.size())
            return &m_columns[n];
        return nullptr;
    }
    
    size_t          IdModel::columnCount() const
    {
        return m_columns.size();
    }
    
    int             IdModel::columnCount(const QModelIndex&) const 
    {
        return m_columns.size();
    }
    
    QVariant        IdModel::data(const QModelIndex&idx, int role) const 
    {
        const IdColumn* col   = column((size_t) idx.column());
        if(!col)
            return QVariant();

        const Node*   n   = node(idx);
        if(!n)
            return QVariant();

        switch(role){
        case Qt::DisplayRole:
            if(!n->id)
                return col -> addMsg;
            if(col->fnDisplay)
                return col->fnDisplay(n->id);
            break;
        case Qt::EditRole:
            if(!n->id)
                return col -> defVal;
            if(col->fnEdit)
                return col->fnEdit(n->id);
            if(col->fnDisplay)
                return col->fnDisplay(n->id);
            break;
        case Qt::DecorationRole:
            if(col->fnDecoration)
                return col->fnDecoration(n->id);
            break;
        default:
            break;
        }
        return QVariant();
    }
    
    Qt::ItemFlags   IdModel::flags(const QModelIndex& idx) const 
    {
        const IdColumn* col   = column((size_t) idx.column());
        if(!col)
            return Qt::NoItemFlags;
        return Qt::NoItemFlags;
    }
    
    QVariant        IdModel::headerData(int n, Qt::Orientation ori, int role) const 
    {
        if(role != Qt::DisplayRole) 
            return QVariant();
        switch(ori){
        case Qt::Horizontal:
            if(n>=0 && n<(int) m_columns.size())
                return m_columns[n].label;
            break;
        case Qt::Vertical:
            return n+1;
        }
        return QVariant();
    }

    QModelIndex     IdModel::index(int row, int column, const QModelIndex&idx) const 
    {
        const Node* pNode   = node(idx);
        return createIndex(row, column, pNode->children[row]);
    }

    IdModel::Node*  IdModel::node(const QModelIndex&idx)
    {
        if(idx.isValid())
            return (IdModel::Node*) idx.internalPointer();
        else
            return &m_root;
    }
    
    const IdModel::Node*     IdModel::node(const QModelIndex&idx) const
    {
        if(idx.isValid())
            return (const IdModel::Node*) idx.internalPointer();
        else
            return &m_root;
    }

    QModelIndex     IdModel::parent(const QModelIndex&child) const 
    {
        const Node* n   = node(child);
        if(!n)
            return QModelIndex();
        const Node* pn  = n->parent;
        if(!pn)
            return QModelIndex();
        const Node* gpn = pn->parent;
        if(!gpn)
            return QModelIndex();
        size_t row = gpn->indexOf(n);
        return createIndex(row, child.column(), pn);
    }

    void            IdModel::reload()
    {
        auto rr = resetModel();
        _load();
    }

    IdModel::EndCue   IdModel::resetModel()
    {
        beginResetModel();
        return EndCue(this, &IdModel::endResetModel);
    }

    
    int             IdModel::rowCount(const QModelIndex& idx) const 
    {
        const Node* n   = node(idx);
        return n->children.size();
    }

    void    IdModel::setColumn(IdColumn&&col)
    {
        if(m_columns.empty()){
            m_columns.push_back(col);
        } else {
            m_columns[0]        = std::move(col);
        }
    }

    void            IdModel::setColumn(size_t n, IdColumn&&col)
    {
        if(n<m_columns.size()){
            m_columns[n]        = std::move(col);
            m_columns[n].number = n;
        } else {
            col.number  = m_columns.size();
            m_columns.push_back(col);
        }
    }

    void    IdModel::setColumns(std::vector<IdColumn>&& cols)
    {
        m_columns       = std::move(cols);
        for(size_t n=0;n<m_columns.size();++n)
            m_columns[n].number = n;
    }
    
    bool            IdModel::setData(const QModelIndex&idx, const QVariant&, int ) 
    {
        const IdColumn* col   = column((size_t) idx.column());
        if(!col)
            return false;
            
        return false;
    }
    

    void            IdModel::setFilters(std::vector<IdFilter>&& filters)
    {
        m_root.filters  = std::move(filters);
    }

    void            IdModel::setFilters(const QModelIndex&idx, std::vector<IdFilter>&&filters)
    {
        Node*   n   = node(idx);
        n->filters  = std::move(filters);
    }
    
    void            IdModel::update(const QModelIndex&)
    {
        reload();   // TODO (get fancier with diffs)
    }

////////////////////////////////////////////////////////////////////////////////

    IdModel::EndCue::EndCue()
    {
    }

    IdModel::EndCue::EndCue(EndCue&& mv)
    {
        _move(std::move(mv));
    }
    
    IdModel::EndCue& IdModel::EndCue::operator=(EndCue&&mv)
    {
        if(this != &mv){
            _dtor();
            _move(std::move(mv));
        }
        return *this;
    }
    
    IdModel::EndCue::~EndCue()
    {
        _dtor();
    }
    
    IdModel::EndCue::EndCue(IdModel* im,FN fn) : m_model(im), m_function(fn) 
    {
    }

    void IdModel::EndCue::_dtor()
    {
        if(m_model && m_function){
            (m_model->*m_function)();
            m_model     = nullptr;
            m_function  = nullptr;
        }
    }
    
    void IdModel::EndCue::_move(EndCue&&mv)
    {
        m_model     = mv.m_model;
        m_function  = mv.m_function;
        mv.m_model      = nullptr;
        mv.m_function   = nullptr;
    }

////////////////////////////////////////////////////////////////////////////////

    IdModel::Node::Node(IdModel*m, Id  i, Node*p, IdProvider&& ipu) : 
        id(i), model(m), parent(p), provider(std::move(ipu))
    {
    }
    
    IdModel::Node::~Node()
    {
        purge();
    }

    std::vector<Id>     IdModel::Node::fetch() const
    {
        std::vector<Id> ret;
        if(provider){
            std::vector<Id> them    = provider();
            if(filters.empty()){
                ret = std::move(them);
            } else {
                ret.reserve(them.size());
                for(Id i : them){
                    bool    rej = false;
                    for(auto& f : filters){
                        if(!f(i)){
                            rej = true;
                            break;
                        }
                    }
                    if(!rej)
                        ret.push_back(i);
                }
            }
            
        }
        return ret;
    }
    
    size_t  IdModel::Node::indexOf(const Node*n) const
    {
        for(size_t i=0;i<children.size();++i)
            if(children[i] == n)
                return i;
        return children.size();
    }

    void    IdModel::Node::purge()
    {
        for(Node*n : children)
            delete n;
        children.clear();
        ids.clear();
    }

    void    IdModel::Node::reload()
    {
        purge();
        auto d  = provider();
        children.reserve(d.size());
        for(Id i : d){
            if(!filters.empty()){
                bool    rej    = false;
                for(auto& f : filters)
                    rej = rej || f(i);
                if(rej)
                    continue;
            }
            
            IdProvider    upp;
            bool                branch  = false;
            if(model->isTree() && model->m_treeGenerator && model->m_treeDetect && model->m_treeDetect(i)){
                upp     = std::move(model->m_treeGenerator(i));
                branch  = upp ? true : false;
            }
            
            Node*   ch  = new Node(model, i, this, std::move(upp));
            if(branch)
                ch -> reload();
            ids.insert(i);
            children.push_back(ch);
        }
    }

}
