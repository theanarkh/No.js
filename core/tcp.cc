#include "tcp.h"      

void No::TCP::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> tcp = ObjectTemplate::New(isolate);
  setMethod(isolate, tcp, "listen", No::Net::Listen);
  setMethod(isolate, tcp, "accept", No::Net::Accept);
  setMethod(isolate, tcp, "socket", No::Net::Socket);
  setMethod(isolate, tcp, "bind", No::Net::Bind);
  setMethod(isolate, tcp, "close", No::IO::Close);
  setMethod(isolate, tcp, "connect", No::Net::Connect);
  setMethod(isolate, tcp, "setsockopt", No::Net::Setsockopt);
  setMethod(isolate, tcp, "read", No::IO::Read);
  setMethod(isolate, tcp, "write", No::IO::Write);
  setObjectValue(isolate, target, "tcp", tcp->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

