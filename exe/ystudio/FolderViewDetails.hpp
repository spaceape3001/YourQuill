////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QTableView>
#include <QAbstractTableModel>

class FolderViewDetails : public QTableView {
    Q_OBJECT
public:

    class Model;
};

class FolderViewDetails::Model : public QAbstractTableModel {
    Q_OBJECT
public:
    
};
