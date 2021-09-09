#ifndef INOTIFY_H
#define INOTIFY_H

#include <sys/inotify.h>
#include "include/common.h"
#include <vector>
#include <map>
#include <memory>
#include "util.h"
#include "env.h"
#include "string.h"
#include "io_uring.h"
#include "async.h"

using namespace No::Util;
using namespace No::Env;
using namespace No::io_uring;
using namespace No::Async;
using namespace std;

static IdGenerator globalIdGenerator;
static bool needSubmit = true;
static map<int, vector<shared_ptr<InotifyRequestContext>>> inotifyMap;

namespace No {
    namespace Inotify {
        void On(V8_ARGS);
        void Off(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif