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

        //  call when the columns have all been defined
        void    finalize();

    signals:
        void    popupRequested(Id);

    protected:
        IdModel*    m_model = nullptr;

        void contextMenuEvent(QContextMenuEvent*) override;
    };
}
