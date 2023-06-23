////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Dock.hpp"
#include <mithrilQt/id/IdModel.hpp>
#include <mithrilQt/id/IdTableView.hpp>

class XClipboard : public Dock {
    Q_OBJECT
    YQ_OBJECT_DECLARE(XClipboard, Dock)
public:    

    class Model;
    class View;

    XClipboard(QWidget*parent=nullptr);
    ~XClipboard();

    Model*          model() { return m_model; }
    const Model*    model() const { return m_model; }
    
    View*           view() { return m_view; }
    const View*     view() const { return m_view; }
    
private:
    Model*      m_model = nullptr;
    View*       m_view  = nullptr;
};


class XClipboard::Model : public yq::mithril::IdModel {
    Q_OBJECT
public:
    Model(QObject* parent=nullptr);
    ~Model();
};

class XClipboard::View : public yq::mithril::IdTableView {
    Q_OBJECT
public:
    View(Model*, QWidget*parent=nullptr);
    ~View();
    
    Model*          model();
    const Model*    model() const;
};

