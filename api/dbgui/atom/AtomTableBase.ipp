////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

template class StdTableModel<Atom>;
template class U64TableModel<Atom>;
template class StdTableView<Atom>;

AtomTableModel::AtomTableModel(AtomProvider ctp, QObject*parent) : 
    U64TableModel<Atom>(ctp?ctp:provider::all_atoms(), parent)
{
    customRO("Key", [](Atom f) -> auto { return cdb::key(f); });
    customRO("Label", [](Atom f) -> auto { return cdb::label(f); });
    customRO("Description", [](Atom f) -> auto { return cdb::brief(f); });
}

AtomTableModel::~AtomTableModel()
{
}


AtomTableView::AtomTableView(AtomTableModel*ctm, QWidget*parent) : StdTableView<Atom>(ctm, parent), m_model(ctm)
{
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

AtomTableView::~AtomTableView()
{
}


