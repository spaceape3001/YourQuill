////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/edit/ComboBox.hpp>
#include <db/DataRole.hpp>

class Root;

class RootComboBox : public ComboBox {
    Q_OBJECT
public:
    RootComboBox(QWidget*parent=nullptr);
    ~RootComboBox();
    
    DataRole    dataRole() const { return m_role; }
    bool        writableRoots() const { return m_writable; }
    
    void        setDataRole(DataRole);
    void        setWritableRoots(bool);
    const Root* currentRoot() const;
    void        setCurrentRoot(const Root*);
    

private:
    void        _update();

    DataRole    m_role;
    bool        m_writable;
};


