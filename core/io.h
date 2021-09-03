#ifndef IO_H
#define IO_H

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
#include "async.h"
#include "string.h"

using namespace No::Env;
using namespace No::io_uring;
using namespace v8;
using namespace No::Util;
using namespace No::Async;

namespace No {
    namespace IO {
        void Read(V8_ARGS);
        void Write(V8_ARGS);
        void ReadV(V8_ARGS);
        void WriteV(V8_ARGS);
        void Inherit(Isolate* isolate, Local<ObjectTemplate> target);
        void Init(Isolate* isolate, Local<Object> target);
    }
}
#endif 
