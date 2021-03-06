#include "net.h"      

void No::Net::Socket(V8_ARGS) {
    V8_ISOLATE
    int domain = args[0].As<Integer>()->Value(); 
    int type = args[1].As<Integer>()->Value(); 
    int fd = socket(domain, type, 0);
    V8_RETURN(Integer::New(isolate, fd));
}

static struct sockaddr_in HandleAddrInfo(V8_ARGS) {
    V8_ISOLATE
    int fd = args[0].As<Integer>()->Value(); 
    String::Utf8Value ip(isolate, args[1]); 
    int port = args[2].As<Integer>()->Value(); 
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(*ip);
    serv_addr.sin_port = htons(port);
    return serv_addr;
}

void No::Net::Bind(V8_ARGS) {   
    V8_ISOLATE 
    int fd = args[0].As<Integer>()->Value();
    struct sockaddr_in server_addr_info = HandleAddrInfo(args);
    int ret = bind(fd, (struct sockaddr*)&server_addr_info, sizeof(server_addr_info));
    V8_RETURN(Integer::New(isolate, ret));
}

void No::Net::Connect(V8_ARGS) {  
    V8_ISOLATE
    int fd = args[0].As<Integer>()->Value();
    struct sockaddr_in peer_addr_info = HandleAddrInfo(args);
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
    struct io_uring_info *io_uring_data = env->GetIOUringData();
    // 申请内存
    struct connect_request *req = (struct connect_request *)malloc(sizeof(struct connect_request));
    // to do
    memset(req, 0, sizeof(*req));
    req->fd = fd;
    req->addr = (struct sockaddr *)&peer_addr_info;
    req->addrlen = sizeof(peer_addr_info);
    req->op = IORING_OP_CONNECT;
    req->cb = makeCallback<onconnect>;
    if (args.Length() > 3 && args[3]->IsFunction()) {
        Local<Object> obj = Object::New(isolate);
        Local<String> event = newStringToLcal(isolate, onconnect);
        obj->Set(context, event, args[3].As<Function>());
	    req->data = (void *)new RequestContext(env, obj);
    } else {
        req->data = (void *)new RequestContext(env, Local<Function>());
    }
    SubmitRequest((struct request*)req, io_uring_data);
}

void No::Net::Setsockopt(V8_ARGS) {    
    int fd = args[0].As<Integer>()->Value();
    int level = args[1].As<Integer>()->Value();
    int name = args[2].As<Integer>()->Value();
    int value = args[3].As<Integer>()->Value();
    socklen_t len = sizeof(value);
    setsockopt(fd, level, name, (void *)&value, len);
}

void No::Net::Listen(V8_ARGS) {
    V8_ISOLATE
    int fd = args[0].As<Integer>()->Value(); 
    int backlog = args[0].As<Integer>()->Value(); 
    int ret = listen(fd, backlog);
    if (ret == -1) {
        V8_RETURN(Integer::New(isolate, ret));
        return;
    }
    struct tcp_request *req = (struct tcp_request *)malloc(sizeof(struct tcp_request));  
     memset(req, 0, sizeof(*req));
    req->fd = fd;
    req->op = IORING_OP_ACCEPT;
    req->cb = makeCallback<onconnect>;
    req->flag = IO_URING_REQUEST_AGAIN;
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
     if (args.Length() > 2 && args[2]->IsFunction()) {
        Local<Object> obj = Object::New(isolate);
        Local<String> event = newStringToLcal(isolate, onconnect);
        obj->Set(context, event, args[2].As<Function>());
	    req->data = (void *)new RequestContext(env, obj);
    } else {
        req->data = (void *)new RequestContext(env, Local<Function>());
    }
    struct io_uring_info *io_uring_data = env->GetIOUringData();
    SubmitRequest((struct request *)req, io_uring_data);
    V8_RETURN(Integer::New(isolate, ret));
}

void No::Net::Accept(V8_ARGS) {
    V8_ISOLATE
    int fd = args[0].As<Integer>()->Value(); 
    int clientFd = accept(fd, nullptr, nullptr);
    V8_RETURN(Integer::New(isolate, clientFd));;
}

void No::Net::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> net = ObjectTemplate::New(isolate);
  Local<ObjectTemplate> constant = ObjectTemplate::New(isolate);
  {
    Local<ObjectTemplate> domain = ObjectTemplate::New(isolate);
    setObjectTemplateValue(isolate, domain, "AF_UNIX", Number::New(isolate, AF_UNIX));
    setObjectTemplateValue(isolate, domain, "AF_INET", Number::New(isolate, AF_INET));
    setObjectTemplateValue(isolate, domain, "AF_INET6", Number::New(isolate, AF_INET6));
    setObjectTemplateValue(isolate, constant, "domain", domain);
  }

  {
    Local<ObjectTemplate> type = ObjectTemplate::New(isolate);
    setObjectTemplateValue(isolate, type, "SOCK_STREAM", Number::New(isolate, SOCK_STREAM));
    setObjectTemplateValue(isolate, type, "SOCK_DGRAM", Number::New(isolate, SOCK_DGRAM));
    setObjectTemplateValue(isolate, type, "SOCK_RAW", Number::New(isolate, SOCK_RAW));  
    setObjectTemplateValue(isolate, constant, "type", type);
  }
  
  {
    Local<ObjectTemplate> level = ObjectTemplate::New(isolate);
    setObjectTemplateValue(isolate, level, "SOL_SOCKET", Number::New(isolate, SOL_SOCKET));
    setObjectTemplateValue(isolate, constant, "level", level);
  }

  {
    Local<ObjectTemplate> socketOption = ObjectTemplate::New(isolate);
    #define MODE_LIST(Set) \
        Set(SO_DEBUG) \
        Set(SO_REUSEADDR) \
        Set(SO_TYPE) \
        Set(SO_ERROR) \
        Set(SO_DONTROUTE) \
        Set(SO_BROADCAST) \
        Set(SO_SNDBUF) \
        Set(SO_RCVBUF) \
        Set(SO_SNDBUFFORCE) \
        Set(SO_RCVBUFFORCE) \
        Set(SO_KEEPALIVE) \
        Set(SO_OOBINLINE) \
        Set(SO_NO_CHECK) \
        Set(SO_PRIORITY) \
        Set(SO_LINGER) \
        Set(SO_BSDCOMPAT) \
        Set(SO_REUSEPORT) 
    #define Set(val) setObjectTemplateValue(isolate, socketOption, #val, Number::New(isolate, val));
        MODE_LIST(Set)
    #undef Set
  #undef MODE_LIST
    
    setObjectTemplateValue(isolate, constant, "socketOption", socketOption);
  }
    

  setObjectTemplateValue(isolate, net, "constant", constant);
  setObjectValue(isolate, target, "net", net->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

