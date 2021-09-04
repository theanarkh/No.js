#include "unix_domain.h"
#include <stdio.h>

void No::UNIX_DOMAIN::SocketPair(V8_ARGS) {
    V8_ISOLATE
    int domain = args[0].As<Integer>()->Value(); 
    int type = args[1].As<Integer>()->Value(); 
    int fds[2];
    int ret = socketpair(domain, type, 0, fds);
    if (ret == -1) {
        V8_RETURN(Array::New(isolate));
    } else {
        Local<Value> arr[] = {
            Number::New(isolate, fds[0]),
            Number::New(isolate, fds[1])
        };
        V8_RETURN(Array::New(isolate, arr, 2));
    }
}

static struct sockaddr_un HandleAddrInfo(V8_ARGS) {
    V8_ISOLATE
    String::Utf8Value path(isolate, args[1]); 
    struct sockaddr_un serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, *path);
    return serv_addr;
}

void No::UNIX_DOMAIN::Bind(V8_ARGS) {   
    V8_ISOLATE 
    int fd = args[0].As<Integer>()->Value();
    struct sockaddr_un server_addr_info = HandleAddrInfo(args);
    unlink(server_addr_info.sun_path);
    int ret = bind(fd, (struct sockaddr*)&server_addr_info, sizeof(server_addr_info));
    V8_RETURN(Integer::New(isolate, ret));
}

void No::UNIX_DOMAIN::Connect(V8_ARGS) {  
    V8_ISOLATE
    int fd = args[0].As<Integer>()->Value();
    struct sockaddr_un peer_addr_info = HandleAddrInfo(args);
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
    struct io_uring_info *io_uring_data = env->GetIOUringData();
    // 申请内存
    struct connect_request *req = (struct connect_request *)malloc(sizeof(struct connect_request));
    // to do
    memset(req, 0, sizeof(struct connect_request));
    req->fd = fd;
    req->addr = (struct sockaddr *)&peer_addr_info;
    req->addrlen = sizeof(peer_addr_info);
    req->op = IORING_OP_CONNECT;
    req->cb = makeCallback<onconnect>;
    if (args.Length() > 2 && args[2]->IsFunction()) {
        Local<Object> obj = Object::New(isolate);
        Local<String> key = newStringToLcal(isolate, onconnect);
        obj->Set(context, key, args[2].As<Function>());
	    req->data = (void *)new RequestContext(env, obj);
    } else {
        req->data = (void *)new RequestContext(env, Local<Function>());
    }
    SubmitRequest((struct request*)req, io_uring_data);
}


void No::UNIX_DOMAIN::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> unix_domain = ObjectTemplate::New(isolate);
  setMethod(isolate, unix_domain, "bind", No::UNIX_DOMAIN::Bind);
  setMethod(isolate, unix_domain, "connect", No::UNIX_DOMAIN::Connect);
  setMethod(isolate, unix_domain, "socketpair", No::UNIX_DOMAIN::SocketPair);
  setMethod(isolate, unix_domain, "listen", No::Net::Listen);
  setMethod(isolate, unix_domain, "accept", No::Net::Accept);
  setMethod(isolate, unix_domain, "socket", No::Net::Socket);
  setMethod(isolate, unix_domain, "close", No::Net::Close);
  setMethod(isolate, unix_domain, "setsockopt", No::Net::Setsockopt);
  setObjectValue(isolate, target, "unix_domain", unix_domain->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

