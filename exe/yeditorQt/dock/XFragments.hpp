////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Dock.hpp"
#include <mithrilQt/id/IdModel.hpp>
#include <mithrilQt/id/IdTreeView.hpp>


class XFragments : public ZDock {
    Q_OBJECT
public:    

    class Model;
    class View;

    XFragments(QWidget*parent=nullptr);
    ~XFragments();
    
    Model*          model() { return m_model; }
    const Model*    model() const { return m_model; }
    
    View*           view() { return m_view; }
    const View*     view() const { return m_view; }
    
private:
    Model*      m_model = nullptr;
    View*       m_view  = nullptr;
};

class XFragments::Model : public yq::mithril::IdModel {
    Q_OBJECT
public:
    Model(QObject* parent=nullptr);
    ~Model();
};

class XFragments::View : public yq::mithril::IdTreeView {
    Q_OBJECT
public:
    View(Model*, QWidget*parent=nullptr);
    ~View();
    
    Model*          model();
    const Model*    model() const;
};

