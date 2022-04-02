////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yquill.hpp"

#include <yq/collection/Vector.hpp>

#include <chrono>
#include <condition_variable>
#include <mutex>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using namespace yq;

namespace {
    void    dir_monitor()
    {
        using namespace std::chrono_literals;
        //uint64_t    cnt = 0;
        //  TODO....
        while(gQuit == Quit::No){
            std::this_thread::sleep_for(5s);
            //yInfo() << "Dir monitor thread, tick " << ++cnt;
        }
    }

}

void        run_scanner(Vector<std::thread>& threads)
{
    std::condition_variable cv;
    std::mutex              mutex;
    bool                    ready = false;
    
    threads << std::thread([&](){
        thread::id();
        ready       = true;
        cv.notify_one();
        dir_monitor();
    });
    
    {
        std::unique_lock lk(mutex);
        cv.wait(lk, [&]{ return ready; });
    }
}

