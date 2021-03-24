#pragma once
#include <QWidget>
#include "gui/SubWin.hpp"
#include "leaf/AtomSys.hpp"

class ClassEdit;

class ClassEditor : public SubWin {
    Q_OBJECT
public:
    ClassEditor(Class, const Root*, QWidget*parent=nullptr);
    ~ClassEditor();
    
private:
    Class           m_class;
    const Root*     m_root;
    ClassEdit*      m_edit;
};

