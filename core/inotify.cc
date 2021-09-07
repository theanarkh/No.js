#include "inotify.h"

void No::Inotify::On(Isolate* isolate, Local<Object> target) {
    V8_ISOLATE
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
    String::Utf8Value path(isolate, args[0]);
    int mask = args[1].As<Integer>()->Value();
    int id = inotify_add_watch(env->getInotifyFd(), *path, mask);
    if (id == -1) {
        V8_RETURN(Integer::New(isolate, id));
        return;
    }
    Local<Object> obj = Object::New(isolate);
    Local<String> key = newStringToLcal(isolate, onchange);
    obj->Set(context, key,  args[2].As<Function>());
    
    auto vec = inotifyMap.find(inotifyInfo);
    shared_ptr ctx = make_shared<InotifyRequestContext>(env, obj);
    if (vec == inotifyMap.end()) {
        InotifyInfo inotifyInfo(*path, id);
        vector<shared_ptr<InotifyRequestContext> v;
        v.push_back(ctx);
        inotifyMap.insert(map<InotifyInfo,  vector<shared_ptr<InotifyRequestContext>>::value_type(inotifyInfo, v));
    }
    vec->second.push_back(ctx);
    V8_RETURN(Integer::New(isolate, 0));
}

void No::Inotify::Off(Isolate* isolate, Local<Object> target) {
    V8_ISOLATE
    Environment *env = Environment::GetEnvByContext(isolate->GetCurrentContext());
    String::Utf8Value path(isolate, args[0]);
    inotifyMap.erase(id);
    auto listener = inotifyMap.find(*path);
    if (listener != inotifyMap.end()) {
        listener->second;
        int wd = args[0].As<Integer>()->Value();
        int id = inotify_rm_watch(env->getInotifyFd(), wd);
        if (id == -1) {
            V8_RETURN(Integer::New(isolate, id));
            return;
        }
    }
    V8_RETURN(Integer::New(isolate, 0));
}

void No::Inotify::Init(Isolate* isolate, Local<Object> target) {
    int fd = inotify_init1(0);
    Environment *env = Environment::GetEnvByContext(isolate->GetCurrentContext());
    env->setInotifyFd(fd);
    Local<ObjectTemplate> inotify = ObjectTemplate::New(isolate);
    setMethod(isolate, inotify, "on", No::Inotify::On);
    setMethod(isolate, inotify, "off", No::Inotify::Off);
    setObjectValue(isolate, target, "inotify", inotify->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}