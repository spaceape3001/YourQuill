#include "stdquill.hpp"

namespace {
    void    var_can_edit(Stream& str, WebContext&)
    {
        //  TODO
        str << true;
    }

    void    var_local(Stream& str, WebContext& ctx)
    {
        str << ctx.is_local();
    }

    void    var_logged_in(Stream&str, WebContext& ctx)
    {
        //  TODO
        str << true;
    }

    void    var_ssid(Stream&str, WebContext& ctx)
    {
        str << ctx.session.ssid;
    }

}


YQ_INVOKE(
    reg_webvar<var_can_edit>("can_edit");
    reg_webvar<var_logged_in>("logged_in");
    reg_webvar<var_local>("local");
    reg_webvar<var_ssid>("ssid");
)
