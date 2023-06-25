////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QListView>

namespace yq::mithril {
    class IdModel;

    class IdListView : public QListView {
        Q_OBJECT
    public:
        IdListView(IdModel*, QWidget*parent=nullptr);
        ~IdListView();

    signals:
        void    popupRequested(Id);

    private:
        IdModel*    m_model = nullptr;

        void contextMenuEvent(QContextMenuEvent*) override;
    };
}
