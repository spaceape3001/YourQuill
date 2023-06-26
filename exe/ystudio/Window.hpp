////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ystudio.hpp"
#include <gluon/app/SubWindow.hpp>

class DreamMW;

class Window : public yq::gluon::SubWindow {
    Q_OBJECT
public:

    enum class Mode {
        //! Unknown/no mode for this item
        None    = 0,
        
        //! Viewing the item
        View,
        
        //! Editing the item
        Edit
    };
    
    //! If there's an item of focus, this is it
    Id      item() const { return m_item; }
    Mode    mode() const { return m_mode; }

signals:
    void    popupRequested(Id, const QActionList& acts = QActionList());
    void    openRequested(Id);
    
protected:
    Window(QWidget*parent=nullptr);
    ~Window();
    
    DreamMW*    dream() const;

        // Set the below to advertise
    Id      m_item = {};
    Mode    m_mode  = Mode::None;
};

