#include "core.h"

void No::Core::register_builtins(Isolate * isolate, Local<Object> target) {
    FS::Init(isolate, target); 
    TCP::Init(isolate, target); 
    Process::Init(isolate, target); 
    Console::Init(isolate, target);
    IO::Init(isolate, target);
    Net::Init(isolate, target);
    UDP::Init(isolate, target);
    UNIX_DOMAIN::Init(isolate, target);
    Signal::Init(isolate, target);
    Timer::Init(isolate, target);
}

void No::Core::Run(io_uring_info * io_uring_data) {
    io_uring::RunIOUring(io_uring_data);
}   