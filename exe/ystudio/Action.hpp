////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QIcon>
#include <QKeySequence>

class QAction;

struct ActionInfo {
    QString         label;
    QString         toolTip;
    QKeySequence    shortcut;
    QIcon           icon;
    bool            checkable   = false;
    bool            checked     = false;
};

QAction*    createAction(const ActionInfo&, QObject*parent=nullptr);


