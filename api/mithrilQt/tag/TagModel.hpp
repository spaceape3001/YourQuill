////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithrilQt/preamble.hpp>
#include <optional>

namespace yq::mithril {
    class TagModel : public IdModelT<Tag> {
        Q_OBJECT
    public:
    
        TagModel(Type t, QObject* parent=nullptr) : TagModel(t, ALL, parent) {}
        TagModel(Type t, all_t, QObject* parent=nullptr);
        ~TagModel();
        
    private:
        TagModel(Type t, Tag, IdProvider&&, QObject*parent);
    };
}
