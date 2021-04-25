#include "ClassEditor.hpp"
#include "edit/ClassEdit.hpp"
#include "leaf/Wksp.hpp"
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>

ClassEditor::ClassEditor(Class cls, const Root*r, QWidget*parent) : SubWin(parent), m_class(cls), m_root(r)
{
    m_edit = new ClassEdit;

    QVBoxLayout*    lay = new QVBoxLayout(this);
    
    lay -> setContentsMargins(0,0,0,0);
    lay -> addWidget(m_edit, 1);

    setWindowTitle(tr("Class %1 (%2)").arg(cls.name()).arg(m_root->name()));
    
}

ClassEditor::~ClassEditor()
{
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_ClassEditor.cpp"
