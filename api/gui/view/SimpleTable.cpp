////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SimpleTable.hpp"
#include <QHeaderView>


SimpleTable::SimpleTable(QWidget*parent) : QTableView(parent)
{
    m_model     = new Model(this);
    setModel(m_model);
    horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

bool SimpleTable::set(int r,int c,const QVariant&value)
{
    return m_model->set(r,c,value);
}

void    SimpleTable::setHeader(const Vector<QString>&h)
{
    m_model->setHeader(h);
}

void    SimpleTable::clear() 
{
    m_model->clear();
}

//void        SimpleTable::fit()
//{
//}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SimpleTable::Model::Model(QObject* parent) : QAbstractTableModel(parent)
{
}

SimpleTable::Model::~Model()
{
}


void        SimpleTable::Model::setHeader(const Vector<QString>& h)
{
    beginResetModel();
    m_headers = h;
    m_data.resize(0,h.size());
    endResetModel();
}

QVariant    SimpleTable::Model::data(const QModelIndex&idx, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    int r   = idx.row();
    int c   = idx.column();
    if(m_data.isValid(r,c))
        return m_data(r,c);
    return QVariant();
}

QVariant    SimpleTable::Model::headerData(int n, Qt::Orientation ori, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(ori == Qt::Horizontal)
        return m_headers.at(n);
    else
        return 1+n;
}


bool SimpleTable::Model::set(int r,int c,const QVariant&value)
{
    if(r < 0)
        return false;
    if(c < 0)
        return false;
    if(c >= m_data.columnCount())
        return false;

    if(r >= m_data.rowCount()){
        int     n   = m_data.rowCount();
        beginInsertRows(QModelIndex(), n, r);
        m_data.append_rows(r-m_data.rowCount() + 1);
        endInsertRows();
    }

    m_data(r,c) = value;
    QModelIndex idx= createIndex(r,c);
    emit dataChanged(idx,idx);
    return true;
}


void    SimpleTable::Model::clear()
{
    beginResetModel();
    m_data.resize(0,m_headers.size());
    endResetModel();
}

#include "moc_SimpleTable.cpp"

