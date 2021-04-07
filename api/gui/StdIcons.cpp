////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <QIcon>
#include <util/Utilities.hpp>

QIcon   si_app_exit()
{
    static thread_local QIcon   ret = theme_icon("application-exit", ":icon/refresh.svg");
    return ret;
}

QIcon   si_edit_clear()
{
    static thread_local QIcon   ret = theme_icon("edit-clear");
    return ret;
}

QIcon   si_edit_copy()
{
    static thread_local QIcon   ret = theme_icon("edit-copy");
    return ret;
}

QIcon   si_edit_cut()
{
    static thread_local QIcon   ret = theme_icon("edit-cut");
    return ret;
}

QIcon   si_edit_delete()
{
    static thread_local QIcon   ret = theme_icon("edit-delete");
    return ret;
}

QIcon   si_edit_find()
{
    static thread_local QIcon   ret = theme_icon("edit-find");
    return ret;
}

QIcon   si_edit_paste()
{
    static thread_local QIcon   ret = theme_icon("edit-paste");
    return ret;
}

QIcon   si_edit_redo()
{
    static thread_local QIcon   ret = theme_icon("edit-redo");
    return ret;
}

QIcon   si_edit_replace()
{
    static thread_local QIcon   ret = theme_icon("edit-find-replace");
    return ret;
}

QIcon   si_edit_select_all()
{
    static thread_local QIcon   ret = theme_icon("edit-select-all");
    return ret;
}

QIcon   si_edit_undo()
{
    static thread_local QIcon   ret = theme_icon("edit-undo");
    return ret;
}

QIcon   si_folder()
{
    static thread_local QIcon   ret = theme_icon("folder", ":icon/folder.svg");
    return ret;
}


QIcon   si_go_back()
{
    static thread_local QIcon ret = theme_icon("go-previous", ":icon/left.svg");
    return ret;
}

QIcon   si_go_forward()
{
    static thread_local QIcon ret = theme_icon("go-next", ":icon/right.svg");
    return ret;
}

QIcon   si_go_home()
{
    static thread_local QIcon   ret = theme_icon("go-home", ":icon/home.svg");
    return ret;
}

QIcon   si_go_up()
{
    static thread_local QIcon ret   = theme_icon("go-up", ":icon/up.svg");
    return ret;
}

QIcon   si_list_add()
{
    static thread_local QIcon   ret = theme_icon("list-add", ":icon/add.svg");
    return ret;
}

QIcon   si_list_down()
{
    static thread_local QIcon ret(":icon/down.svg");
    return ret;
}

QIcon   si_list_remove()
{
    static thread_local QIcon   ret = theme_icon("list-remove", ":icon/remove.svg");
    return ret;
}

QIcon   si_list_up()
{
    static thread_local QIcon ret(":icon/up.svg");
    return ret;
}

QIcon   si_view_refresh()
{
    static thread_local QIcon   ret = theme_icon("view-refresh", ":icon/refresh.svg");
    return ret;
}

