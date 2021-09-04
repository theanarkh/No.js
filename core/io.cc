#include "io.h"      

void read_write_request(V8_ARGS, int op) { 
    V8_ISOLATE
    int fd = args[0].As<Uint32>()->Value();
    int offset = 0;
    if (args.Length() > 2 && args[2]->IsNumber()) {
        offset = args[2].As<Integer>()->Value();
    }
    Local<ArrayBuffer> arrayBuffer = args[1].As<ArrayBuffer>();
    std::shared_ptr<BackingStore> backing = arrayBuffer->GetBackingStore();
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
    struct io_uring_info *io_uring_data = env->GetIOUringData();
    struct request *req;
    bool isBatch = op == IORING_OP_READV || op == IORING_OP_WRITEV;
    if (isBatch) {
        struct io_batch_request *io_batch_req = (struct io_batch_request *)malloc(sizeof(struct io_batch_request) + (sizeof(struct iovec) * 1));
        io_batch_req->nvecs = 1;
        io_batch_req->iovecs[0].iov_len = backing->ByteLength();
        io_batch_req->iovecs[0].iov_base = backing->Data();
        io_batch_req->fd = fd;
        io_batch_req->offset = offset;
        req = (struct request *)io_batch_req;
    } else {
        struct io_request *io_req = (struct io_request *)malloc(sizeof(struct io_request) + (sizeof(struct iovec) * 1));
        io_req->buf = backing->Data();
        io_req->len = backing->ByteLength();
        io_req->fd = fd;
        io_req->offset = offset;
        req = (struct request *)io_req;
    }
    req->cb = makeCallback<onread>;
    req->op = op;
    if (args.Length() > 3 && args[3]->IsFunction()) {
        Local<Object> obj = Object::New(isolate);
        Local<String> key = newStringToLcal(isolate, onread);
        obj->Set(context, key, args[3].As<Function>());
	    req->data = (void *)new RequestContext(env, obj);
    } else {
        req->data = (void *)new RequestContext(env, Local<Function>());
    }
    SubmitRequest((struct request *)req, io_uring_data);  
}

void No::IO::Read(V8_ARGS) {
    read_write_request(args, IORING_OP_READ);
}

void No::IO::Write(V8_ARGS) {
    read_write_request(args, IORING_OP_WRITE);
}

void No::IO::ReadV(V8_ARGS) {
    read_write_request(args, IORING_OP_READV);
}

void No::IO::WriteV(V8_ARGS) {
    read_write_request(args, IORING_OP_WRITEV);
}

void No::IO::Inherit(Isolate* isolate, Local<ObjectTemplate> target) {
  setMethod(isolate, target, "read", No::IO::Read);
  setMethod(isolate, target, "write", No::IO::Write);
  setMethod(isolate, target, "readv", No::IO::ReadV);
  setMethod(isolate, target, "writev", No::IO::WriteV);
}

void No::IO::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> io = ObjectTemplate::New(isolate);
  No::IO::Inherit(isolate, io);
  setObjectValue(isolate, target, "io", io->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

