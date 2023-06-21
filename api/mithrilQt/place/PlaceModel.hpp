////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/place/Place.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class PlaceModel : public IdModelT<Place> {
        Q_OBJECT
    public:

        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());
    
        PlaceModel(Type t, QObject* parent=nullptr) : PlaceModel(t, ALL, parent) {}
        PlaceModel(Type t, all_t, QObject* parent=nullptr);
        ~PlaceModel();
        
        using IdModelT<Place>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);

        using IdModelT<Place>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());
        
    private:
        PlaceModel(Type t, Place, IdProvider&&, QObject*parent);
    };
}
