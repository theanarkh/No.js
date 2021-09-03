
#include "util.h"

void No::Util::Log(const char * str) {
    write(STDOUT_FILENO, str, strlen(str));
}

Local<String> No::Util::newStringToLcal(Isolate * isolate, const char * str, NewStringType type) {
    return String::NewFromUtf8(isolate, str, type, strlen(str)).ToLocalChecked();
}

MaybeLocal<String> No::Util::newString(Isolate * isolate, const char * str, NewStringType type) {
    return String::NewFromUtf8(isolate, str, type, strlen(str));
}

// 设置对象的属性，属性为函数
void No::Util::setMethod(Isolate *isolate, 
                Local<ObjectTemplate> 
                recv, const char *name, 
                FunctionCallback callback) {
    recv->Set(String::NewFromUtf8(isolate, name, 
        NewStringType::kNormal).ToLocalChecked(), 
        FunctionTemplate::New(isolate, callback));
}

// 设置对象的属性，属性为非函数
void No::Util::setObjectTemplateValue(Isolate *isolate, 
                Local<ObjectTemplate> 
                recv, const char *name, 
                // TEMPLATE OR PRIVATE
                Local<Data> value) {
    recv->Set(String::NewFromUtf8(isolate, name, 
        NewStringType::kNormal).ToLocalChecked(), 
        value);
}

// 设置对象的属性，属性为非函数
void No::Util::setObjectValue(Isolate *isolate, 
                Local<Object> 
                recv, const char *name, 
                Local<Value> value) {
    recv->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, name, 
        NewStringType::kNormal).ToLocalChecked(), 
        value);
}

// template <class TypeName>
Local<Context> No::Util::PersistentToLocal::Strong(const PersistentBase<Context>& persistent) {
    return *reinterpret_cast<Local<Context>*>(
        const_cast<PersistentBase<Context>*>(&persistent)
    );
}

char * No::Util::GetErrorByErrno() {
    return strerror(errno);
}
