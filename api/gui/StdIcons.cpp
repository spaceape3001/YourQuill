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

QIcon   si_mime_bmp()
{
    static thread_local QIcon ret(":mime/bmp.svg");
    return ret;
}

QIcon   si_mime_css()
{
    static thread_local QIcon ret(":mime/css.svg");
    return ret;
}

QIcon   si_mime_gif()
{
    static thread_local QIcon ret(":mime/gif.svg");
    return ret;
}

QIcon   si_mime_html()
{
    static thread_local QIcon ret(":mime/html.svg");
    return ret;
}

QIcon   si_mime_jpg()
{
    static thread_local QIcon ret(":mime/jpg.svg");
    return ret;
}

QIcon   si_mime_png()
{
    static thread_local QIcon ret(":mime/png.svg");
    return ret;
}

QIcon   si_mime_rtf()
{
    static thread_local QIcon ret(":mime/rtf.svg");
    return ret;
}

QIcon   si_mime_tif()
{
    static thread_local QIcon ret(":mime/tif.svg");
    return ret;
}

QIcon   si_mime_txt()
{
    static thread_local QIcon ret(":mime/txt.svg");
    return ret;
}

QIcon   si_mime_wiki()
{
    static thread_local QIcon ret(":mime/wiki.svg");
    return ret;
}


QIcon   si_view_refresh()
{
    static thread_local QIcon   ret = theme_icon("view-refresh", ":icon/refresh.svg");
    return ret;
}

