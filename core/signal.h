#ifndef SIGNAL_H
#define SIGNAL_H

#include <signal.h>
#include <memory>
#include <map> 
#include <vector>
#include "util.h"
#include "include/common.h"
#include "env.h"
#include "string.h"
#include "async.h"

using namespace v8;
using namespace No::Util;
using namespace No::Async;
using namespace No::Env;
using namespace std;

static map<int, vector<shared_ptr<SignalRequestContext>>> signalMap;

namespace No {
    namespace Signal {
        void RegisterSignal(V8_ARGS);
        void UnRegisterSignal(V8_ARGS);
        void Kill(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif