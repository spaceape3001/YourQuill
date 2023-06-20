////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/place/Place.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class PlaceModel : public IdModelT<Place> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        PlaceModel(Type t) : PlaceModel(t, ALL) {}
        PlaceModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~PlaceModel();
        
        using IdModelT<Place>::addColumn;
        void    addColumn(Column);
        
    private:
        PlaceModel(Type t, Place, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
