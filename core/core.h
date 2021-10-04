#ifndef CORE_H
#define CORE_H

#include "include/common.h"
#include "tcp.h"
#include "fs.h"
#include "process.h"
#include "net.h"
#include "io.h"
#include "udp.h"
#include "ipc/unix_domain.h"
#include "console.h"
#include "io_uring.h"
#include "signal.h"
#include "timer.h"
#include "inotify.h"
#include "loader.h"
#include "util.h"
#include "vm.h"
#include "http.h"

using namespace v8;
using namespace No;
using namespace No::Env;

namespace No {
    namespace Core {
        void register_builtins(Isolate * isolate, Local<Object> target);
        void Run(struct io_uring_info * io_uring_data);
    }
}

#endif