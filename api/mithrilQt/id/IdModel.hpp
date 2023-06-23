////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractItemModel>
#include <gluon/delegate/Delegate.hpp>
#include <mithril/id/Id.hpp>
#include <mithrilQt/id/IdColumn.hpp>
#include <gluon/core/Utilities.hpp>

class QModelIndex;
class QVariant;

namespace yq::mithril {
    class IdColumn;
    class IdTable;
    template <typename> class IdTableT;
    
    class IdModel : public QAbstractItemModel {
        Q_OBJECT
    public:
    
        using TreeDetector          = std::function<bool(Id)>;
        using ProviderGenerator     = std::function<IdProvider(Id)>;
        using VariantFN             = IdColumn::VariantFN;
    
        enum class AddPolicy {
            None,
            Signal,
            Create
        };
        
        enum class Type {
            None,
            List,
            Table,
            Tree
        };

        void            addColumn(ColumnSpec);
        void            addColumn(IdColumn&&);
        void            addColumn(Column, ColOpts={});
        void            addColumn(size_t before, IdColumn&&);

        template <cdb_object S, typename T>
        void            addColumn(std::string_view label, std::function<T(S)>);

        void            addColumns(std::span<const Column>);
        void            addColumns(std::span<const ColumnSpec>);
    
        const IdColumn* column(size_t) const;
        size_t          columnCount() const;
        int             columnCount(const QModelIndex&) const override;
        QVariant        data(const QModelIndex&, int role) const override;
        Qt::ItemFlags   flags(const QModelIndex&) const override;
        QVariant        headerData(int, Qt::Orientation, int) const override;
        QModelIndex     index(int row, int column, const QModelIndex&parent) const override;
        QModelIndex     parent(const QModelIndex&) const override;
        int             rowCount(const QModelIndex& idx=QModelIndex()) const override;
        bool            setData(const QModelIndex&, const QVariant&, int ) override;

        bool            isList() const { return m_type == Type::List; }
        bool            isTable() const { return m_type == Type::Table; }
        bool            isTree() const { return m_type == Type::Tree; }

        void            setColumn(ColumnSpec);
        void            setColumn(Column, ColOpts={});
        void            setColumn(IdColumn&&);
        void            setColumn(size_t, IdColumn&&);

        void            setColumns(std::vector<IdColumn>&&);
        
        //! Sets new filters (for all queries)
        //! \note Call a reload after setting filters
        void            setFilters(std::vector<IdFilter>&&);
        
        //! Sets new filters for specific node
        void            setFilters(const QModelIndex&, std::vector<IdFilter>&&);
        
        //! Adds a filter to all queries
        void            addFilter(IdFilter&&);
        
        //! Adds a filter to a specific node
        void            addFilter(const QModelIndex&, IdFilter&&);

        //! Reset on this!
        void            setProvider(IdProvider&&);

        const std::vector<IdColumn>& columns() const { return m_columns; }


        class EndCue;
        
        //! If you're doing something big... reset and save the end-cue to destruct (it'll call end())
        [[nodiscard]] EndCue   resetModel();

        IdModel(Type, Id, IdProvider&&, QObject*parent=nullptr);

    public slots:
        //! Reload/reset the list with current data
        void            reload();
        
        //! Updates the list with current contents
        void            update(const QModelIndex& idx=QModelIndex());

    protected:
    
        friend class IdTable;
        template <typename> friend class IdTableT;
        
        struct Node {
            Id const                    id;
            IdModel* const              model;
            Node* const                 parent;
            IdProvider                  provider;
            
            std::vector<Node*>          children;
            std::set<Id>                ids;
            std::vector<IdFilter>       filters;
            
            Node(IdModel*, Id, Node*p, IdProvider&& ipu={});
            ~Node();
            void                        purge();
            
            size_t                      indexOf(const Node*) const;
            
            Id                          child(size_t) const;
            
            std::vector<Id>             fetch() const;
            void                        reload();
        };
    
    
        Type                            m_type              = Type::None;
        TreeDetector                    m_treeDetect;
        ProviderGenerator               m_treeGenerator;
        Node                            m_root;
        std::vector<IdColumn>           m_columns;
        std::vector<IdFilter>           m_filters;
        AddPolicy                       m_addPolicy     = AddPolicy::None;
        bool                            m_readOnly      = false;
        bool                            m_anyColAdd     = false;    // true if ANY column can trigger adds
        bool                            m_dropEnabled   = false;
        bool                            m_showVHeader   = true;
        bool                            m_showHHeader   = true;
        VariantFN                       m_vHeader;
        IdType                          m_idType        = IdType::Unknown;
        
        virtual ~IdModel();
        
        Node*           node(const QModelIndex&);
        const Node*     node(const QModelIndex&) const;
        
        
        

        void            _load();
        void            _updateCols();

    };
    
    class IdModel::EndCue {
    public:

        EndCue(EndCue&&);
        EndCue& operator=(EndCue&&);
        ~EndCue();
    
    private:
        friend class IdModel;
        
        typedef void (IdModel::*FN)();
    
        IdModel*        m_model     = nullptr;
        FN              m_function  = nullptr;
        
        EndCue();
        EndCue(IdModel* im,FN fn);
        
        EndCue(const EndCue&) = delete;
        EndCue& operator=(const EndCue&) = delete;
        
        void _dtor();
        void _move(EndCue&&);
    };

    template <cdb_object S, typename T>
    void    IdModel::addColumn(std::string_view label, std::function<T(S)> fn)
    {
        using namespace yq::gluon;
        IdColumn    col;
        col.label       = QString::fromUtf8(label.data(), label.size());
        col.fnDisplay   = [fn](Id d) -> QVariant {
            if( d.type() != id_type_v<S> )
                return QVariant();
            S   s(d.id());
            if constexpr (std::is_same_v<T,std::string_view> || std::is_same_v<T,std::string>){
                return QVariant(qString(fn(s)));
            } else {
                return QVariant::fromValue(fn(s));
            }
        };
        
        if constexpr (QMetaTypeId<T>::Defined){
            using namespace yq::gluon;
            const DelegateInfo* di  = DelegateInfo::byQtType(qMetaTypeId<T>());
            if(di){
                col.fnDelegate      = [di]()->Delegate* {
                    return static_cast<Delegate*>(di->create());
                };
            }
        }
        
        addColumn(std::move(col));
    }
    
    template <typename S>
    class IdModelT : public IdModel {
    public:
        static IdProvider toProvider(std::function<std::vector<S>()> fn)
        {
            return [fn]() -> std::vector<Id>{
                return ids(fn());
            };
        }

        static IdProvider toProvider(S rt, std::function<std::vector<S>(S)> fn)
        {
            return [rt, fn]() -> std::vector<Id>{
                return ids(fn(rt));
            };
        }

        IdModelT(Type t, IdProvider&& p, QObject*parent=nullptr) :
            IdModelT(t, S(), std::move(p), parent) {}
        
        IdModelT(Type t, S s, IdProvider&& p, QObject*parent=nullptr) :
            IdModel(t, s, std::move(p), parent) 
        {
            m_idType    = id_type_v<S>;
        }

        void    setVHeader(std::function<QVariant(S)>fn)
        {
            m_vHeader  = [fn](Id i){
                return fn(i.as<S>());
            };
        }

        template <typename T>
        void    makeColumn(std::string_view label, std::function<T(S)> fn)
        {
            addColumn<S,T>(label, fn);
        }
    };
    
}

