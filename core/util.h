#ifndef Util_H
#define Util_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/common.h"

using namespace v8;

namespace No {
    namespace Util {
        char * GetErrorByErrno();
        void Log(const char * str);
        Local<String> newStringToLcal(Isolate * isolate, const char * str, NewStringType type = NewStringType::kNormal);
        MaybeLocal<String> newString(Isolate * isolate, const char * str, NewStringType type= NewStringType::kNormal);
        // 设置对象的属性，属性为函数
        void setMethod(Isolate *isolate, Local<ObjectTemplate> recv, const char *name, FunctionCallback callback);
        void setObjectTemplateValue(Isolate *isolate, Local<ObjectTemplate> recv, const char *name, Local<Data> value);
        void setObjectValue(Isolate *isolate, Local<Object> recv, const char *name, Local<Value> value);
        class PersistentToLocal {
            public:
//                template <class TypeName>
                static Local<Context> Strong(const PersistentBase<Context>& persistent);
        };
    }
}

#endif 
