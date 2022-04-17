#pragma once

namespace {
    void    var_body(Stream& str, WebContext&ctx)
    {
        str << ctx.var_body;
    }

    YQ_INVOKE(reg_webvar<var_body>("body");)
}
