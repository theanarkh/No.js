#ifndef VM_H
#define VM_H

#include "include/common.h"
#include "util.h"
#include "env.h"
#include <vector>
#include <unistd.h>
#include <iostream>

enum MODE {
    CODE_MODE = 1,
    FILE_MODE = 2
};

using namespace v8;
using namespace No::Util;
using namespace No::Env;

namespace No {
    namespace VM {
        void Run(V8_ARGS);
        void CompileFunction(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}
#endif 
