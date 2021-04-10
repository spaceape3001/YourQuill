////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


template class StdTableModel<Document>;
template class U64TableModel<Document>;
template class StdTableView<Document>;

DocumentTableModel::DocumentTableModel(DocumentProvider ctp, QObject*parent) : 
    U64TableModel<Document>(ctp?ctp:provider::all_documents(), parent)
{
    customRO("Name", [](Document d) -> auto { return cdb::name(d); });
    customRO("Frags", [](Document d) -> auto { return (quint64) cdb::fragments_count(d); });
    customRO("Key", [](Document d) -> auto { return cdb::key(d); });
}

DocumentTableModel::~DocumentTableModel()
{
}


DocumentTableView::DocumentTableView(DocumentTableModel*ctm, QWidget*parent) : StdTableView<Document>(ctm, parent), m_model(ctm)
{
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

DocumentTableView::~DocumentTableView()
{
}

