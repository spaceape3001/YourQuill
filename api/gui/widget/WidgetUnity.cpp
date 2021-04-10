////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DrawRuler.hpp"
#include "FormWidget.hpp"
#include "GridTickModel.hpp"
#include "StandardGridTickModel.hpp"
#include "TabWidget.hpp"
#include "TabWidgetBar.hpp"

#include "util/Vector.hpp"

//#include <QColor>
#include <QBoxLayout>
#include <QDebug>
#include <QDragEnterEvent>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMetaMethod>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QStackedLayout>
#include <QToolButton>
#include <QVariant>
#include <QVBoxLayout>

//#include "util/Map.hpp"

#include <math.h>
#include <string.h>

Q_DECLARE_METATYPE(QWidget*)

namespace {
    QHBoxLayout*    makeLayout(const std::initializer_list<QWidget*>& widgets, bool fStretchLast)
    {
        QHBoxLayout*    ret = new QHBoxLayout;
        size_t          n   = widgets.size() - 1;   // Yes, possible underflow, however, not used if widgets is empty
        size_t          i   = 0;
        for(QWidget* w : widgets){
            ret -> addWidget(w, (fStretchLast && (i != n)) ? 0 : 1);
            ++i;
        }
        return ret;
    }
}


#include "DrawRuler.ipp"
#include "FormWidget.ipp"
#include "GridTickModel.ipp"
#include "TabWidget.ipp"
#include "TabWidgetBar.ipp"

#include "moc_FormWidget.cpp"


#include "moc_DrawRuler.cpp"
#include "moc_GridTickModel.cpp"
#include "moc_StandardGridTickModel.cpp"
#include "moc_TabWidget.cpp"
#include "moc_TabWidgetBar.cpp"
