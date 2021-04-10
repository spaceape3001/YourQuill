////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


template class StdTableModel<Attribute>;
template class U64TableModel<Attribute>;
template class StdTableView<Attribute>;

AttributeTableModel::AttributeTableModel(AttributeProvider cpp, QObject*parent) : 
    U64TableModel<Attribute>(cpp?cpp:provider::all_attributes(), parent)
{
    customRO("Key", [](Attribute c) -> auto { return cdb::key(c); });
    customRO("Value", [](Attribute c) -> auto { return cdb::value(c); });
}

AttributeTableModel::~AttributeTableModel()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


AttributeTableView::AttributeTableView(AttributeTableModel*ctm, QWidget*parent) : StdTableView<Attribute>(ctm, parent), m_model(ctm)
{
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

AttributeTableView::~AttributeTableView()
{
}

