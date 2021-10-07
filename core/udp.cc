#include "udp.h"      

void No::UDP::SendTo(V8_ARGS) {
    V8_ISOLATE
    int fd = args[0].As<Integer>()->Value(); 
    Local<Uint8Array> uint8Array = args[1].As<Uint8Array>();
    Local<ArrayBuffer> arrayBuffer = uint8Array->Buffer();
    std::shared_ptr<BackingStore> backing = arrayBuffer->GetBackingStore();
    int flags = args[2].As<Integer>()->Value(); 
    const struct sockaddr_in * dest_addr_ptr = nullptr;
    socklen_t addrlen = 0;
    if (args.Length() > 4) {
        struct sockaddr_in dest_addr;
        String::Utf8Value ip(isolate, args[3]);
        int port = args[4].As<Integer>()->Value();
        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_addr.s_addr = inet_addr(*ip);
        dest_addr.sin_port = htons(port);
        dest_addr_ptr = &dest_addr;
        addrlen = sizeof(dest_addr);
    }
    ssize_t ret = sendto(fd, backing->Data(), backing->ByteLength(), flags,
                (struct sockaddr *)dest_addr_ptr, addrlen); 
    V8_RETURN(Integer::New(isolate, ret));
}

void No::UDP::RecvFrom(V8_ARGS) {
    V8_ISOLATE
    int fd = args[0].As<Integer>()->Value(); 
    Local<Uint8Array> uint8Array = args[1].As<Uint8Array>();
    Local<ArrayBuffer> arrayBuffer = uint8Array->Buffer();
    std::shared_ptr<BackingStore> backing = arrayBuffer->GetBackingStore();
    int flags = args[2].As<Integer>()->Value(); 
    const struct sockaddr_in * src_addr_ptr = nullptr;
    socklen_t addrlen = 0;
    if (args.Length() > 4) {
        struct sockaddr_in src_addr;
        String::Utf8Value ip(isolate, args[3]);
        int port = args[4].As<Integer>()->Value();
        memset(&src_addr, 0, sizeof(src_addr));
        src_addr.sin_family = AF_INET;
        src_addr.sin_addr.s_addr = inet_addr(*ip);
        src_addr.sin_port = htons(port);
        src_addr_ptr = &src_addr;
        addrlen = sizeof(src_addr);
    }
    ssize_t ret = recvfrom(fd, backing->Data(), backing->ByteLength(), flags,
                (struct sockaddr *)src_addr_ptr, &addrlen);
    V8_RETURN(Integer::New(isolate, ret));            
}

void No::UDP::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> udp = ObjectTemplate::New(isolate);
  setMethod(isolate, udp, "sendto", No::UDP::SendTo);
  setMethod(isolate, udp, "recvfrom", No::UDP::RecvFrom);
  setMethod(isolate, udp, "socket", No::Net::Socket);
  setMethod(isolate, udp, "bind", No::Net::Bind);
  setMethod(isolate, udp, "close", No::IO::Close);
  setMethod(isolate, udp, "connect", No::Net::Connect);
  setMethod(isolate, udp, "setsockopt", No::Net::Setsockopt);
  setMethod(isolate, udp, "readSync", No::IO::ReadSync);
  setMethod(isolate, udp, "writeSync", No::IO::WriteSync);
  setObjectValue(isolate, target, "udp", udp->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

