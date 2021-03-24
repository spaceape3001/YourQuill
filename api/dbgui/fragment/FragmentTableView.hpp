////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Fragment;
class FragmentTableModel;

class FragmentTableView : public StdTableView<Fragment> {
    Q_OBJECT
public:
    FragmentTableView(FragmentTableModel*, QWidget*parent=nullptr);
    ~FragmentTableView();
    
    FragmentTableModel*    model() const { return m_model; }
    
private:
    FragmentTableModel*    m_model;
};
