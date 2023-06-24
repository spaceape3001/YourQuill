////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Dock.hpp"
#include <mithrilQt/id/IdModel.hpp>
#include <mithrilQt/id/IdTableView.hpp>


using yq::mithril::Id;
using yq::mithril::IdProvider;
using yq::mithril::IdModel;
using yq::mithril::IdTableView;


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
    Model*              m_model = nullptr;
    View*               m_view  = nullptr;
    std::vector<Id>     m_data;
    
    void        append(std::span<const Id>);
};


class XClipboard::Model : public IdModel {
    Q_OBJECT
public:
    Model(IdProvider&&, QObject* parent=nullptr);
    ~Model();
};

class XClipboard::View : public IdTableView {
    Q_OBJECT
public:
    View(Model*, QWidget*parent=nullptr);
    ~View();
    
    Model*          model();
    const Model*    model() const;
};

