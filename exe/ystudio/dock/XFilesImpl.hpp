////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "XFiles.hpp"
#include <mithrilQt/id/IdModel.hpp>
#include <mithrilQt/id/IdTreeView.hpp>


class XFiles::Model : public yq::mithril::IdModel {
    Q_OBJECT
public:
    Model(QObject* parent=nullptr);
    ~Model();
};

class XFiles::View : public yq::mithril::IdTreeView {
    Q_OBJECT
public:
    View(Model*, QWidget*parent=nullptr);
    ~View();
    
    Model*          model();
    const Model*    model() const;
};

