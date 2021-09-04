#include "fs.h"
#include <stdio.h>

void No::FS::Open(V8_ARGS) {
  V8_ISOLATE
  String::Utf8Value filename(isolate, args[0]);
  int argc = args.Length();
  int flags = O_RDONLY;
  if (argc > 1) flags = Local<Integer>::Cast(args[1])->Value();
  int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if (argc > 2) mode = Local<Integer>::Cast(args[2])->Value();
  Local<Integer> ret = Integer::New(isolate, open(*filename, flags, mode));
  V8_RETURN(ret);
}

void openAfter(void * req) {
    struct openat_file_request * file_req = (struct openat_file_request *)req;
    printf("%d", file_req->res);
    free(req);
}

void No::FS::OpenAt(V8_ARGS) {
  V8_ISOLATE
  int dirFd = args[0]->IsNull() ? AT_FDCWD : Local<Integer>::Cast(args[0])->Value();
  String::Utf8Value path(isolate, args[1]);
  int argc = args.Length();
  int flags = O_RDONLY;
  if (argc > 2) flags = Local<Integer>::Cast(args[2])->Value();
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if (argc > 3) mode = Local<Integer>::Cast(args[3])->Value();
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
    struct io_uring_info *io_uring_data = env->GetIOUringData();
    // 申请内存
    struct openat_file_request *req = (struct openat_file_request *)malloc(sizeof(struct openat_file_request) + (sizeof(struct iovec) * 1));
    memset(req, 0, sizeof(*req));
    req->dirFd = dirFd; 
    req->path = *path;
    req->mode = mode;
    req->flags = flags;
    req->cb = openAfter;
    req->op = IORING_OP_OPENAT;

    if (args.Length() > 4 && args[4]->IsFunction()) {
        req->data = (void *)new RequestContext(env, args[4].As<Function>());
    } else {
        req->data = (void *)new RequestContext(env, Local<Function>());
    }
    SubmitRequest((struct request *)req, io_uring_data); 
}

void No::FS::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> fs = ObjectTemplate::New(isolate);
  setMethod(isolate, fs, "open", No::FS::Open);
  setMethod(isolate, fs, "openat", No::FS::OpenAt);
  setMethod(isolate, fs, "close", No::IO::Close);
  setMethod(isolate, fs, "read", No::IO::Read);
  setMethod(isolate, fs, "write", No::IO::Write);
  setMethod(isolate, fs, "readv", No::IO::ReadV);
  setMethod(isolate, fs, "writev", No::IO::WriteV);
  setObjectValue(isolate, target, "fs", fs->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}



