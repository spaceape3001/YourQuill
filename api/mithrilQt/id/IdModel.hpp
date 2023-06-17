////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractItemModel>
#include <mithril/id/Id.hpp>
#include <mithril/id/IdProvider.hpp>
#include <mithril/id/IdFilter.hpp>

class QModelIndex;
class QVariant;

namespace yq::mithril {
    class IdColumn;
    

    class IdModel : public QAbstractItemModel {
        Q_OBJECT
    public:
    
        using TreeDetector          = std::function<bool(Id)>;
        using ProviderGenerator     = std::function<IdProvider::UPtr(Id)>;
    
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
    
        //! Sets new filters (for the root node)
        //! \note Call a reload after setting filters
        void            setFilters(std::vector<IdFilter::UPtr>&&);
        void            setFilters(const QModelIndex&, std::vector<IdFilter::UPtr>&&);
    
        const IdColumn* column(size_t) const;
        size_t          columnCount() const;
        int             columnCount(const QModelIndex&) const override;
        QVariant        data(const QModelIndex&, int role) const override;
        Qt::ItemFlags   flags(const QModelIndex&) const override;
        QVariant        headerData(int, Qt::Orientation, int) const override;
        QModelIndex     index(int row, int column, const QModelIndex&parent) const override;
        QModelIndex     parent(const QModelIndex&) const override;
        int             rowCount(const QModelIndex&) const override;
        bool            setData(const QModelIndex&, const QVariant&, int ) override;

        bool            isList() const { return m_type == Type::List; }
        bool            isTable() const { return m_type == Type::Table; }
        bool            isTree() const { return m_type == Type::Tree; }

    public slots:
        //! Reload/reset the list with current data
        void            reload();
        
        //! Updates the list with current contents
        void            update(const QModelIndex& idx=QModelIndex());

    protected:
        
        struct Node {
            Id const                    id;
            IdModel* const              model;
            Node* const                 parent;
            IdProvider::UPtr const      provider;
            
            std::vector<Node*>          children;
            std::set<Id>                ids;
            std::vector<IdFilter::UPtr> filters;
            
            Node(IdModel*, Id, Node*p, IdProvider::UPtr&& ipu={});
            ~Node();
            void                        purge();
            
            size_t                      indexOf(const Node*) const;
            
            std::vector<Id>             fetch() const;
            void                        reload();
        };
    
        Type                            m_type              = Type::None;
        TreeDetector                    m_treeDetect;
        ProviderGenerator               m_treeGenerator;
        Node                            m_root;
        std::vector<IdColumn>           m_columns;
        AddPolicy                       m_addPolicy = AddPolicy::None;
        bool                            m_readOnly  = false;
        
        virtual ~IdModel();
        
        Node*           node(const QModelIndex&);
        const Node*     node(const QModelIndex&) const;
        
        
        class EndCue;
        
        [[nodiscard]] EndCue   resetModel();
        
        IdModel(Type, Id, IdProvider::UPtr&&, QObject*parent=nullptr);

        void            _load();

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

}

