#ifndef NET_H
#define NET_H

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
#include "io.h"
#include "io_uring.h"
#include "async.h"
#include "string.h"

using namespace No::Env;
using namespace No::io_uring;
using namespace v8;
using namespace No::Util;
using namespace No::Async;

namespace No {
    namespace Net {
        void Socket(V8_ARGS);
        void Bind(V8_ARGS);
        void Connect(V8_ARGS);
        void Setsockopt(V8_ARGS);
        void Listen(V8_ARGS);
        void Accept(V8_ARGS);
        void connect_cb(void * req);
        void Init(Isolate* isolate, Local<Object> target);
    }
}
#endif 
