#ifndef HTTP_H
#define HTTP_H

#include "HTTP_Parser.h"
#include "util.h"
#include "include/common.h"
#include "io_uring.h"
#include "env.h"
#include "baseObject.h"

using namespace v8;
using namespace No::Util;
using namespace No::Env;
using namespace No::io_uring;
using namespace No::Base;

namespace No {
    namespace HTTP {
        class Parser : public BaseObject {
            public:
                Parser(Environment* env, Local<Object> object): BaseObject(env, object) {}
                Parse();
                static void New(const FunctionCallbackInfo<Value>& args); 
        }
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif