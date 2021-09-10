#include "inotify.h"

void SubmitReadRequest(Environment *env);

void inotifyHandler(void * req) {
    struct io_request * inotify_req = (struct io_request * )req;
    char * buf = (char *)inotify_req->buf;
    char size = inotify_req->res;
    char *p;
    struct inotify_event * e;
    int events;
    for (p = buf; p < buf + size; p += sizeof(*e) + e->len) {
        e = (struct inotify_event*)p;
        events = 0;
        if (e->mask & (IN_ATTRIB|IN_MODIFY))
            events |= CHANGE;
        if (e->mask & ~(IN_ATTRIB|IN_MODIFY))
            events |= RENAME;
        auto listeners = inotifyMap.find(e->wd);
        bool haveListener = listeners != inotifyMap.end();
        if (!haveListener) {
            continue;
        }
        vector<shared_ptr<InotifyRequestContext>>::iterator it;
        // check the size every loop because element may be delete in callback
        for(it=listeners->second.begin(); it!=listeners->second.end() && listeners->second.size() > 0 ; it++)
        {
            makeCallback<onchange>((*it).get());
        }
    }
    free(inotify_req);
    needSubmit = true;
    SubmitReadRequest((Environment *)inotify_req->data);
}

void SubmitReadRequest(Environment *env) {
    if (needSubmit) {
        struct io_request * inotify_req = (struct io_request *)malloc(sizeof(*inotify_req));
        memset(inotify_req, 0, sizeof(*inotify_req));
        int size = 4096;
        char * buf = (char *)malloc(size);
        inotify_req->cb = inotifyHandler;
        inotify_req->fd = env->getInotifyFd();
        inotify_req->op = IORING_OP_READ;
        inotify_req->buf = buf;
        inotify_req->len = size;
        inotify_req->data = (void *)env;
        SubmitRequest((struct request *)inotify_req, env->GetIOUringData());
        needSubmit = false;
    }
}

void No::Inotify::On(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
    String::Utf8Value path(isolate, args[0]);
    int mask = IN_ATTRIB | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO;
    if (args.Length() > 2) {
        mask = args[2].As<Integer>()->Value();
    }
    int id = inotify_add_watch(env->getInotifyFd(), *path, mask);
    if (id == -1) {
        V8_RETURN(Integer::New(isolate, id));
        return;
    }
    count++;
    SubmitReadRequest(env);
    Local<Object> obj = Object::New(isolate);
    Local<String> key = newStringToLcal(isolate, onchange);
    obj->Set(context, key,  args[1].As<Function>());
    auto vec = inotifyMap.find(id);
    int funcId = globalIdGenerator.getNextId();
    shared_ptr<InotifyRequestContext> ctx = make_shared<InotifyRequestContext>(env, obj, funcId);
    if (vec == inotifyMap.end()) {
        vector<shared_ptr<InotifyRequestContext>> v;
        v.push_back(ctx);
        inotifyMap.insert(map<int, vector<shared_ptr<InotifyRequestContext>>>::value_type(id, v));
    } else {
        vec->second.push_back(ctx);
    }
    Local<Object> ret = Object::New(isolate);
    ret->Set(context, newStringToLcal(isolate, "watchId"),Number::New(isolate, id));
    ret->Set(context, newStringToLcal(isolate, "funcId"),Number::New(isolate, funcId));
    V8_RETURN(ret);
}

void No::Inotify::Off(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(isolate->GetCurrentContext());
    Local<Object> obj = args[0].As<Object>();
    Local<Value> watchId;
    obj->Get(context, newStringToLcal(isolate,"watchId")).ToLocal(&watchId);
    Local<Value> funcId;
    obj->Get(context, newStringToLcal(isolate, "funcId")).ToLocal(&funcId);
    if (!watchId->IsNumber()) {
        return;
    }
    int id = watchId.As<Integer>()->Value();
    auto listeners = inotifyMap.find(id);
    bool haveListener = listeners != inotifyMap.end();
    vector<shared_ptr<InotifyRequestContext>>::iterator it;
    if (!haveListener) {
        return;
    }
    if (!funcId->IsNumber()) {
        goto REMOVE_WATCH;
    }
    
    for(it=listeners->second.begin();it!=listeners->second.end(); it++)
    {
        if ((*it)->id == funcId.As<Integer>()->Value()) {
            listeners->second.erase(it);
            break;
        }
    }
    if (listeners->second.size() > 0) {
        V8_RETURN(Integer::New(isolate, 0));
        return;
    }
    REMOVE_WATCH:
        inotifyMap.erase(id);
        int ret = inotify_rm_watch(env->getInotifyFd(), id);
        if (ret == 0) {
            if (--count == 0) {
                decPending(env->GetIOUringData());
            }
        }
        V8_RETURN(Integer::New(isolate, ret));
}

void No::Inotify::Init(Isolate* isolate, Local<Object> target) {
    int fd = inotify_init1(0);
    // if fail to init the inotify, we exit and show the error
    if (fd == -1) {
        Log(GetErrorByErrno());
        exit(126);
    }
    Environment *env = Environment::GetEnvByContext(isolate->GetCurrentContext());
    env->setInotifyFd(fd);
    Local<ObjectTemplate> inotify = ObjectTemplate::New(isolate);
    setMethod(isolate, inotify, "on", No::Inotify::On);
    setMethod(isolate, inotify, "off", No::Inotify::Off);
    setObjectValue(isolate, target, "inotify", inotify->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}