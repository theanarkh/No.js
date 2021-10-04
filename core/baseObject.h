#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "util.h"
#include "include/common.h"
#include "env.h"

using namespace v8;
using namespace No::Util;
using namespace No::Env;
namespace No {
    namespace Base {
        class BaseObject {
            public:
                BaseObject(Environment* env, Local<Object> object);
                ~BaseObject();
                Environment* env() const;
                Local<Object> object();
                static void * unwrap(Local<Object> object);
            private: 
                Environment * _env;
                Global<Object> _object;
        };
    }
}

#endif