////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractItemModel>
#include <gluon/delegate/Delegate.hpp>
#include <mithril/id/Id.hpp>
#include <mithril/id/IdCDB.hpp>
#include <mithrilQt/id/IdColumn.hpp>
#include <gluon/core/Utilities.hpp>

class QModelIndex;
class QVariant;

namespace yq::mithril {
    class IdColumn;
    
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

        /*! \brief Adds a column
        
            This is the main column-adding routine.  It blindly
            accepts the given column, moving it into the vector
            of columns.
        */
        void            addColumn(IdColumn&&);

        /*! \brief Adds a column
        
            This is a convience function for adding a column by 
            specification.  It'll call IdColumn::create() for
            you, append it in.
        */
        void            addColumn(ColumnSpec);

        /*! \brief Adds a column
        
            This adds a template based column, giving data back.  
            
            \note   This has less granular control than the IdColumn&&
                    method, but good for fast & dirty
            
            \tparam S   Type of ID object
            \tparam T   Data type
            
            \param[in]  label   Name of the column
            \param[in]  fn      Functor giving data for S
        */
        template <IdType S, typename T>
        void            addColumn(std::string_view label, std::function<T(S)> fn);

        /*! \brief Adds the specified columns
        */
        void            addColumns(std::span<const Column>);

        /*! \brief Adds the specified columns
        */
        void            addColumns(std::span<const ColumnSpec>);
        
        /*! \brief Adds the default table columns
        */
        void            addColumns(default_k);
        
        bool            addEnabled() const;
        
        //! Adds a filter to all queries
        void            addFilter(IdFilter&&);
        
        //! Adds a filter to a specific node
        void            addFilter(const QModelIndex&, IdFilter&&);

        //! Current add policy
        AddPolicy       addPolicy() const { return m_addPolicy; }
        
        /*! \brief Gets the specified column
        
            \note Ownership is not transferred, and the pointer may
                become invalid with the next column add/insert.
                
            \param[in] n    0...N-1 (column count) index into the column vector
            \return Pointer to that element, nullptr if out-of-bounds
        */
        const IdColumn* column(size_t n) const;
        
        //! \brief Number of columns
        size_t          columnCount() const;

        //! \brief Number of columns (see QAbstractItemModel)
        int             columnCount(const QModelIndex&) const override;
        
        //! Reference to our column vector
        const std::vector<IdColumn>& columns() const { return m_columns; }

        //! Creates a delegate for the specified column, hands over ownership
        gluon::Delegate*    createDelegate(int col) const;

        //! Data of the model (see QAbstractItemModel)
        QVariant        data(const QModelIndex&, int role) const override;

        bool            dropEnabled() const;
        
        /*! \brief Drop Policy
        
            This is the drop policy, an enable/disable flag.  It stil
            requires that columns/handlers support dropping.
        */
        bool            dropPolicy() const;

        /*! \brief Item flags for cell
        
            \note Needed for QAbstractItemModel
        */
        Qt::ItemFlags   flags(const QModelIndex&) const override;

        /*! \brief Horizontal Header Enabled
        
            TRUE if the horizontal header's been enabled
        */
        bool            hasHorizontalHeader() const;

        /*! \brief Vertical Header Enabled
        
            TRUE if the vertical header's been enabled
        */
        bool            hasVerticalHeader() const;

        //! Header data (see QAbstractItemModel)
        QVariant        headerData(int, Qt::Orientation, int) const override;
        
        //! Id for given cell
        Id              id(const QModelIndex&) const;
        
        /*! \brief Id Type of this model
        
            \note Unknown will be reflected for models supporing MULTIPLE 
                    ID types -- thus it's not an error or warning to be unknown.
        */
        IdTypeId        idType() const { return m_idType; }
        
        //! Index (structure, see QAbstractItemModel)
        QModelIndex     index(int row, int column, const QModelIndex&parent) const override;

        /*! \brief Inserts a column
            
            Inserts a column before the specified index.
            *IF* before is too high, this will be equivalent to "add"
        */
        void            insertColumn(size_t before, IdColumn&&);

        //! TRUE if this is a list model
        bool            isList() const { return m_type == Type::List; }
        
        //! TRUE if this is a table model
        bool            isTable() const { return m_type == Type::Table; }
        
        //! TRUE if this is a tree model
        bool            isTree() const { return m_type == Type::Tree; }

        //! Parent node (structure, see QAbstractItemModel)
        QModelIndex     parent(const QModelIndex&) const override;
        
        //! TRUE if read-only is enabled
        bool            readOnly() const;

        //! True if the reload capability is enabled
        bool            reloadEnabled() const;

        Id              rootId() const;

        //! Row count (see QAbstractItemModel)
        int             rowCount(const QModelIndex& idx=QModelIndex()) const override;

        //! Sets the default list column
        void            setColumn(default_k);

        //! Sets the FIRST column
        void            setColumn(ColumnSpec);
        //! Sets the FIRST column
        void            setColumn(IdColumn&&);

        //! Sets the specified column
        void            setColumn(size_t, IdColumn&&);

        //! Sets ALL the columns
        void            setColumns(std::vector<IdColumn>&&);
        
        //! Sets data (see QAbstractItemModel)
        bool            setData(const QModelIndex&, const QVariant&, int ) override;
        
        //! Sets the drop policy
        void            setDropPolicy(bool);
        
        //! Sets new filters (for all queries)
        //! \note Call a reload after setting filters
        void            setFilters(std::vector<IdFilter>&&);
        
        //! Sets new filters for specific node
        void            setFilters(const QModelIndex&, std::vector<IdFilter>&&);
        
        //! Reset on this!
        void            setProvider(IdProvider&&);


        //! Enabled the showing of horizontal header data
        void            showHorizontalHeader(bool);

        //! Enabled the showing of vertical  header data
        void            showVerticalHeader(bool);

        void 	        sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

        class EndCue;
        
        //! If you're doing something big... reset and save the end-cue to destruct (it'll call end())
        [[nodiscard]] EndCue   resetModel();


        IdModel(Type, Id, IdProvider&&, QObject*parent=nullptr);
        virtual ~IdModel();

    public slots:
        //! Reload/reset the list with current data
        void            reload();
        
        //! Updates the list with current contents
        void            update(const QModelIndex& idx=QModelIndex());

    protected:
        
        
        enum class F {
            //! Obvious, in read only mode
            ReadOnly    = 0,
            
            ReloadEnabled,
            
            //! TRUE if any columns are detected to be add-capable
            AnyAdd,     
            
            //! TRUE if any columns are detected to be drop-capable
            AnyDrop,
            
            DropPolicy,
            
            HorizontalHeader,
            VerticalHeader
            
        };
        
        
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
        Flags<F>                        m_flags         = {};
        VariantFN                       m_vHeader;
        IdTypeId                        m_idType        = 0;
        
        
        Node*           node(const QModelIndex&);
        const Node*     node(const QModelIndex&) const;
        
        void            _load();
        void            _updateCols();
        void            _changed();
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

    template <IdType S, typename T>
    void    IdModel::addColumn(std::string_view label, std::function<T(S)> fn)
    {
        using namespace yq::gluon;
        IdColumn    col;
        col.label       = QString::fromUtf8(label.data(), label.size());
        col.fnDisplay   = [fn](Id d) -> QVariant {
            if( d.type() != S::ID )
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
            const DelegateMeta* di  = DelegateMeta::byQtType(qMetaTypeId<T>());
            if(di){
                col.fnDelegate      = [di]()->Delegate* {
                    return static_cast<Delegate*>(di->create());
                };
            }
        }
        
        addColumn(std::move(col));
    }
    
    template <IdType S>
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
        
        static IdProvider   allProvider()
        {
            return []() -> std::vector<Id> {
                return cdb::all(S::ID, Sorted::YES);
            };
        }
        
        IdModelT(Type t, all_k, QObject* parent=nullptr) :
            IdModelT(t, S(), allProvider(), parent) {}
        

        IdModelT(Type t, IdProvider&& p, QObject*parent=nullptr) :
            IdModelT(t, S(), std::move(p), parent) {}
        
        IdModelT(Type t, S s, IdProvider&& p, QObject*parent=nullptr) :
            IdModel(t, s, std::move(p), parent) 
        {
            m_idType    = S::ID;
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

