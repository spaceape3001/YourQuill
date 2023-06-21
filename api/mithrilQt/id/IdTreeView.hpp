////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QTreeView>

namespace yq::mithril {
    class IdModel;

    class IdTreeView : public QTreeView {
        Q_OBJECT
    public:
        IdTreeView(IdModel*, QWidget*parent=nullptr);
        ~IdTreeView();
    private:
        IdModel*    m_model = nullptr;
    };
}
