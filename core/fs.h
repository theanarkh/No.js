#ifndef FS_H
#define FS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <cstdlib>

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
        void OpenSync(V8_ARGS);
        void Realpath(V8_ARGS);
        void OpenAt(V8_ARGS);
        void Dup(V8_ARGS);
        void Dup2(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif
