////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "MainWin.hpp"
#include "SimpleEditor.hpp"
#include "SubWin.hpp"
#include "UndoMain.hpp"
#include "UndoWin.hpp"

#include <gui/undo/UndoBase.hpp>
#include <gui/widget/TabWidget.hpp>
#include <gui/widget/TabWidgetBar.hpp>

#include <util/Utilities.hpp>


//#include <QAction>
#include <QCloseEvent>
#include <QDockWidget>
#include <QDrag>
//#include <QDragEnterEvent>
//#include <QDragMoveEvent>
//#include <QDropEvent>
#include <QFileDialog>
//#include <QIcon>
//#include <QKeySequence>
#include <QMessageBox>
//#include <QMdiArea>
//#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMetaMethod>
//#include <QMetaObject>
#include <QMimeData>
//#include <QPixmap>
//#include <QRect>
//#include <QRegion>
//#include <QSignalMapper>
#include <QStatusBar>
#include <QTimer>
#include <QUndoStack>



namespace {
    static const char* szMime_TabDrag = "application/x-window-drag";

    template <typename T>
    T*                   ownerFor(QObject* obj)
    {
        for(QObject* q = obj; q; q = q -> parent()){
            T* w   = qobject_cast<T*>(q);
            if(w)
                return w;
        }
        return nullptr;
    }
}




#include "MainWin.ipp"
#include "SimpleEditor.ipp"
#include "SubWin.ipp"
#include "UndoMain.ipp"
#include "UndoWin.ipp"

#include "moc_MainWin.cpp"
#include "moc_SimpleEditor.cpp"
#include "moc_SubWin.cpp"
#include "moc_UndoMain.cpp"
#include "moc_UndoWin.cpp"
