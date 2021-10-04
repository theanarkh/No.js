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
            BaseObject(Environment* env, Local<Object> object);
            Environment* BaseObject::env();
            private: 
                Environment _env;
        }
    }
}

#endif