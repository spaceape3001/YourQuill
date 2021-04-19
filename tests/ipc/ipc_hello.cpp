////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <ipc/ipcSocket.hpp>
#include <stdlib.h>

int main(int, char*[])
{
    const char*       tmp = getenv("TMPDIR");
    if(!tmp)
        tmp = "/tmp";
    ipc::Socket     out(std::string(tmp) + "/yquill/ipc/broker", false);
    out.txPrint("Hello World!");
    return 0;
}

