////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


template class StdTableModel<Leaf>;  // explicitly instantiate the template here
template class U64TableModel<Leaf>;  // explicitly instantiate the template here
template class StdTableView<Leaf>;  // explicitly instantiate the template here

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LeafTableModel::LeafTableModel(LeafProvider stp, QObject* parent) : 
    U64TableModel<Leaf>(stp ? stp : provider::all_leafs(), parent)
{
    customRO("Key", [](Leaf t) -> auto {
        return cdb::key(t);
    });
    
    customRO("Ttile", [](Leaf t) -> QString {
        return QString::fromStdString(cdb::title(t));
    //}, [](Leaf& t, const QString& k) -> bool {
        //return cdb::set_name(t, k.simplified());
    });
}

LeafTableModel::~LeafTableModel()
{
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


LeafTableView::LeafTableView(LeafTableModel* m, QWidget*parent) : StdTableView<Leaf>(m, parent), m_model(m)
{
    setModel(m);
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

LeafTableView::~LeafTableView()
{
}

