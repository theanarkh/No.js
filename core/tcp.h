#ifndef TCP_H
#define TCP_H

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
namespace No {
    namespace TCP {
        void Init(Isolate* isolate, Local<Object> target);
    }
}
#endif 
