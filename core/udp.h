#ifndef UDP_H
#define UDP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "util.h"
#include "include/common.h"
#include "env.h"
#include "io_uring.h"
#include "net.h"

using namespace No::Env;
using namespace No::io_uring;
using namespace v8;
using namespace No::Util;
using namespace No::Net;

namespace No {
    namespace UDP {
        void SendTo(V8_ARGS);
        void RecvFrom(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}
#endif 
