////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class PlaceModel;
    class PlaceList : public IdListT<Place> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        PlaceList(all_t, QWidget*parent=nullptr);
        PlaceList(all_t, Column col, QWidget*parent=nullptr);
        ~PlaceList();
        
        PlaceModel*          model();
        const PlaceModel*    model() const;
    };
}
