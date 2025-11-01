////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Dock.hpp"
#include <yq/mithrilQt/id/IdModel.hpp>
#include <yq/mithrilQt/id/IdTableView.hpp>


using yq::mithril::Id;
using yq::mithril::IdProvider;
using yq::mithril::IdModel;
using yq::mithril::IdTableView;


class XIdBoard : public ZDock {
    Q_OBJECT
public:    


    class Model;
    class View;

    XIdBoard(QWidget*parent=nullptr);
    ~XIdBoard();

    Model*          model() { return m_model; }
    const Model*    model() const { return m_model; }
    
    View*           view() { return m_view; }
    const View*     view() const { return m_view; }
    
private:
    Model*              m_model = nullptr;
    View*               m_view  = nullptr;
    std::vector<Id>     m_data;
    
    void        append(std::span<const Id>);
};


class XIdBoard::Model : public IdModel {
    Q_OBJECT
public:
    Model(IdProvider&&, QObject* parent=nullptr);
    ~Model();
};

class XIdBoard::View : public IdTableView {
    Q_OBJECT
public:
    View(Model*, QWidget*parent=nullptr);
    ~View();
    
    Model*          model();
    const Model*    model() const;
};

