#ifndef FS_H
#define FS_H

#include <fcntl.h>
#include <unistd.h>
#include "util.h"
#include "include/common.h"
#include "io_uring.h"
#include "env.h"
#include "io.h"

using namespace v8;
using namespace No::Util;
using namespace No::Env;
using namespace No::io_uring;

namespace No {
    namespace FS {
        void Open(V8_ARGS);
        void OpenAt(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif
