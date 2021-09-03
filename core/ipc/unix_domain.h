#ifndef UNIX_DOMAIN_H
#define UNIX_DOMAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include "../util.h"
#include "../include/common.h"
#include "../env.h"
#include "../io_uring.h"
#include "../net.h"
#include "../async.h"
#include "../string.h"

using namespace No::Env;
using namespace No::io_uring;
using namespace v8;
using namespace No::Util;
using namespace No::Net;
using namespace No::Async;

namespace No {
    namespace UNIX_DOMAIN {
        void SocketPair(V8_ARGS);
        void Bind(V8_ARGS);
        void Connect(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}
#endif 
