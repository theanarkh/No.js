#ifndef SIGNAL_H
#define SIGNAL_H

#include "include/common.h"
#include <signal.h>

using namespace v8;

namespace No {
    namespace Signal {
        std::map<int, Global<Function> callback> signalMap; 

        void RegisterSignal(V8_ARGS);
        void UnRegisterSignal(V8_ARGS)
        void Kill(V8_ARGS);
        void Init(Isolate* isolate, Local<ObjectTemplate> target);
    }
}

#endif