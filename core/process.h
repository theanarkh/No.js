#ifndef CHILD_PROCESS_H
#define CHILD_PROCESS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>

#include "util.h"
#include "include/common.h"
#include "env.h"

using namespace v8;
using namespace No::Util;
using namespace No::Env;

namespace No {
    namespace Process {
        void GetEnv(V8_ARGS);
        void Fork(V8_ARGS);
        void Wait(V8_ARGS);
        void Execve(V8_ARGS);
        void Exit(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif 