////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdModel.hpp"
#include "IdColumn.hpp"
#include <gluon/core/Utilities.hpp>

using namespace yq::gluon;

namespace yq::mithril {

    IdModel::IdModel(Type t, Id i, IdProvider&& p, QObject*parent) : 
        QAbstractItemModel(parent), m_type(t), m_root(this, i, nullptr, std::move(p))
    {
        m_flags |= F::HorizontalHeader;
        m_flags |= F::VerticalHeader;
        m_flags |= F::ReloadEnabled;
    }
    
    IdModel::~IdModel()
    {
    }

    void    IdModel::_changed()
    {
        if((!m_root.children.empty()) && (!m_columns.empty())){
            size_t  nm1 =   m_root.children.size()-1;
            emit dataChanged(
                createIndex(0,0,m_root.children[0]),
                createIndex(nm1, m_columns.size()-1, m_root.children[nm1])
            );
        }
    }
    
    void    IdModel::_load()
    {
        m_root.reload();
    }

    void    IdModel::_updateCols()
    {
        m_flags.clear(F::AnyAdd);
        m_flags.clear(F::AnyDrop);
        
        for(size_t n=0; n<m_columns.size(); ++n){
            IdColumn& col   = m_columns[n];
            col.number    = n;
            
            if(col.fnAdd)
                m_flags |= F::AnyAdd;
            if(col.fnDrop || col.fnDropId)
                m_flags |= F::AnyDrop;
        }
    }

    void    IdModel::addColumn(ColumnSpec cspec)
    {
        std::optional<IdColumn> cc  = IdColumn::create(m_idType, cspec.column, cspec.options);
        if(cc)
            addColumn(std::move(*cc));
    }

    void    IdModel::addColumn(IdColumn&& col)
    {
        m_columns.push_back(std::move(col));
        _updateCols();
    }
    
    void            IdModel::addColumns(std::span<const Column> cols)
    {
        for(Column c : cols)
            addColumn(c);
    }
    
    void            IdModel::addColumns(std::span<const ColumnSpec> cols)
    {
        for(auto & c : cols)
            addColumn(c);
    }

    void            IdModel::addColumns(default_k)
    {
        addColumns(IdColumn::defaultTable(m_idType));
    }

    bool            IdModel::addEnabled() const
    {
        return (m_addPolicy != AddPolicy::None) && m_flags[F::AnyAdd];
    }

    
    void            IdModel::addFilter(IdFilter&&filter)
    {
        m_filters.push_back(std::move(filter));
    }
    
    void            IdModel::addFilter(const QModelIndex&idx, IdFilter&&filter)
    {
        Node* n = node(idx);
        n->filters.push_back(std::move(filter));
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

    Delegate*       IdModel::createDelegate(int cid) const
    {
        const IdColumn* col = column((size_t) cid);
        if(!col)
            return nullptr;
        if(col->fnDelegate)
            return (col->fnDelegate)();
        if(col->qtType)
            return Delegate::make(col->qtType);
        return nullptr;
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

    bool            IdModel::dropEnabled() const
    {
        return m_flags[F::DropPolicy] && m_flags[F::AnyDrop];
    }

    bool            IdModel::dropPolicy() const
    {
        return m_flags[F::DropPolicy];
    }
    
    Qt::ItemFlags   IdModel::flags(const QModelIndex& idx) const 
    {
        const IdColumn* col   = column((size_t) idx.column());
        const Node*n    = node(idx);
        
        Qt::ItemFlags       ret   = Qt::NoItemFlags;
        ret |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if(n && col){
            // ret |= Qt::ItemIsDragEnabled //  TODO 
            if(!readOnly()){
                if(col->fnSet)
                    ret |= Qt::ItemIsEditable;
                if(dropEnabled() && (col->fnDrop || col->fnDropId))
                    ret |= Qt::ItemIsDropEnabled;
            }
        } else {
            if(addEnabled() && col->fnAdd)
                ret |= Qt::ItemIsEditable;
        }
        
        return ret;
    }

    bool            IdModel::hasHorizontalHeader() const
    {
        return m_flags[F::HorizontalHeader];
    }
    
    bool            IdModel::hasVerticalHeader() const
    {
        return m_flags[F::VerticalHeader];
    }
    
    QVariant        IdModel::headerData(int n, Qt::Orientation ori, int role) const 
    {
        if(role != Qt::DisplayRole) 
            return QVariant();
        switch(ori){
        case Qt::Horizontal:
            if(hasHorizontalHeader() && n>=0 && n<(int) m_columns.size())
                return m_columns[n].label;
            break;
        case Qt::Vertical:
            if(hasVerticalHeader()){
                Id  i   = m_root.child((size_t) n);
                if(m_vHeader){
                    return m_vHeader(i);
                } else {
                    return (quint64) i.id();
                }
            }
            break;
        }
        return QVariant();
    }
    
    Id              IdModel::id(const QModelIndex& idx) const
    {
        const Node* pNode   = node(idx);
        return pNode -> id;
    }

    QModelIndex     IdModel::index(int row, int column, const QModelIndex&idx) const 
    {
        const Node* pNode   = node(idx);
        return createIndex(row, column, pNode->children[row]);
    }

    void    IdModel::insertColumn(size_t before, IdColumn&& col)
    {
        if(before < m_columns.size()){
            m_columns.insert(m_columns.begin()+before, std::move(col));
        } else {
            m_columns.push_back(std::move(col));
        }
        _updateCols();
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

    bool            IdModel::readOnly() const
    {
        return m_flags[F::ReadOnly];
    }

    void            IdModel::reload()
    {
        if(reloadEnabled()){
            auto rr = resetModel();
            _load();
        }
    }
    
    bool            IdModel::reloadEnabled() const
    {
        return m_flags[F::ReloadEnabled];
    }


    IdModel::EndCue   IdModel::resetModel()
    {
        beginResetModel();
        return EndCue(this, &IdModel::endResetModel);
    }

    Id              IdModel::rootId() const
    {
        return m_root.id;
    }
    
    int             IdModel::rowCount(const QModelIndex& idx) const 
    {
        const Node* n   = node(idx);
        return n->children.size();
    }

    void    IdModel::setColumn(ColumnSpec cspec)
    {
        std::optional<IdColumn> cc  = IdColumn::create(m_idType, cspec.column, cspec.options);
        if(cc)
            setColumn(std::move(*cc));
    }

    void    IdModel::setColumn(default_k)
    {
        setColumn(IdColumn::defaultList(m_idType));
    }

    void    IdModel::setColumn(IdColumn&&col)
    {
        if(m_columns.empty()){
            m_columns.push_back(col);
        } else {
            m_columns[0]        = std::move(col);
        }
        _updateCols();
    }

    void            IdModel::setColumn(size_t n, IdColumn&&col)
    {
        if(n<m_columns.size()){
            m_columns[n]        = std::move(col);
        } else {
            m_columns.push_back(col);
        }
        _updateCols();
    }

    void    IdModel::setColumns(std::vector<IdColumn>&& cols)
    {
        m_columns       = std::move(cols);
        _updateCols();
    }
    
    bool            IdModel::setData(const QModelIndex&idx, const QVariant&, int ) 
    {
        if(readOnly())
            return false;
            
        const IdColumn* col   = column((size_t) idx.column());
        if(!col)
            return false;
            
        return false;
    }

    void            IdModel::setDropPolicy(bool b)
    {
        m_flags.set(F::DropPolicy, b);
    }

    void            IdModel::setFilters(std::vector<IdFilter>&& filters)
    {
        m_filters       = std::move(filters);
    }

    void            IdModel::setFilters(const QModelIndex&idx, std::vector<IdFilter>&&filters)
    {
        Node*   n   = node(idx);
        n->filters  = std::move(filters);
    }

    void            IdModel::setProvider(IdProvider&& prov)
    {
        m_root.provider = std::move(prov);
    }

    void 	        IdModel::sort(int colid, Qt::SortOrder order)
    {
        const IdColumn* col = column(colid);
        if(!col)
            return ;
            
        if(col->fnCompare){
            auto sorter = [&](Node*a, Node*b) -> bool {
                Compare cmp = col->fnCompare(a->id, b->id);
                return (order == Qt::AscendingOrder) ? (cmp == Compare::LESSER) : (cmp == Compare::GREATER);
            };

            std::sort(m_root.children.begin(), m_root.children.end(), sorter);
        } else {
            Delegate*   del = createDelegate(colid);
            auto sorter = [&](Node*a, Node*b) -> bool {
                QVariant    va  = (col->fnSort) ? col->fnSort(a->id) : col->fnDisplay(a->id);
                QVariant    vb  = (col->fnSort) ? col->fnSort(b->id) : col->fnDisplay(b->id);
                Compare cmp = del ? del->compare(va, vb) : yq::gluon::compare(va, vb);
                return (order == Qt::AscendingOrder) ? (cmp == Compare::LESSER) : (cmp == Compare::GREATER);
            };
            
            std::sort(m_root.children.begin(), m_root.children.end(), sorter);
            if(del)
                del -> deleteLater();
        }
        _changed();
    }
    
    
    void            IdModel::update(const QModelIndex&)
    {
        if(reloadEnabled()){
            reload();   // TODO (get fancier with diffs)
        }
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

    Id          IdModel::Node::child(size_t i) const
    {
        if(i>=children.size())
            return Id{};
        const Node* n   = children[i];
        if(!n)
            return Id{};
        return n->id;
    }

    std::vector<Id>     IdModel::Node::fetch() const
    {
        std::vector<Id> ret;
        if(provider){
            std::vector<Id> them    = provider();
            if(filters.empty() && model->m_filters.empty()){
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
                    if(!rej){
                        for(auto& f : model->m_filters){
                            if(!f(i)){
                                rej = true;
                                break;
                            }
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

#include "moc_IdModel.cpp"
