////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QTableView>

namespace yq::mithril {
    class IdModel;

    class IdTableView : public QTableView {
        Q_OBJECT
    public:
        IdTableView(IdModel*, QWidget*parent=nullptr);
        ~IdTableView();
    protected:
        IdModel*    m_model = nullptr;
    };
}
