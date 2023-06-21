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
    private:
        IdModel*    m_model = nullptr;
    };
}
