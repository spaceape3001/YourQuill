////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/CommonDirs.hpp>
#include <util/CmdArgs.hpp>
#include <util/Logging.hpp>
#include <ipc/ipcSocket.hpp>
#include <stdlib.h>
#include <iostream>

int main(int argc, char* argv[])
{
    CmdArgs args(argc, argv);
    log_to_std_error();
    CommonDir::init();
    ipc::Socket     out(gDir.ipc / "broker", false);
    out.txRestart();
    return 0;
}
