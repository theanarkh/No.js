#include "fs.h"

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

void No::FS::Realpath(V8_ARGS) {
  V8_ISOLATE
  String::Utf8Value filename(isolate, args[0]);
  char resolved_path[PATH_MAX];
  const char * path = realpath(*filename, resolved_path);
  if (!path) {
    return V8_RETURN(Null(isolate));
  }
  Local<String> pathObj = newStringToLcal(isolate, path);
  V8_RETURN(pathObj);
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

void No::FS::Dup(V8_ARGS) { 
  V8_ISOLATE 
  int fd = args[0].As<Integer>()->Value();
  V8_RETURN(Integer::New(isolate, dup(fd)));
}

void No::FS::Dup2(V8_ARGS) { 
  V8_ISOLATE 
  int fd1 = args[0].As<Integer>()->Value();
  int fd2 = args[1].As<Integer>()->Value();
  V8_RETURN(Integer::New(isolate, dup2(fd1, fd2)));
}

static void InitConstant(Isolate* isolate, Local<ObjectTemplate> target) {
  
  Local<ObjectTemplate> constant = ObjectTemplate::New(isolate);
  Local<ObjectTemplate> mode = ObjectTemplate::New(isolate);
  Local<ObjectTemplate> flag = ObjectTemplate::New(isolate);

  #define MODE_LIST(Set) \
    Set(S_ISUID) \
    Set(S_ISGID) \
    Set(S_ISVTX) \
    Set(S_IRUSR) \
    Set(S_IWUSR) \
    Set(S_IXUSR) \
    Set(S_IRGRP) \
    Set(S_IWGRP) \
    Set(S_IXGRP) \
    Set(S_IROTH) \
    Set(S_IWOTH) \
    Set(S_IXOTH) 
    #define Set(val) setObjectTemplateValue(isolate, mode, #val, Number::New(isolate, val));
        MODE_LIST(Set)
    #undef Set
  #undef MODE_LIST

   #define FLAG_LIST(Set) \
    Set(O_APPEND) \
    Set(O_ASYNC) \
    Set(O_CLOEXEC) \
    Set(O_CREAT) \
    Set(O_DIRECT) \
    Set(O_DIRECTORY) \
    Set(O_DSYNC) \
    Set(O_EXCL) \
    Set(O_LARGEFILE) \
    Set(O_NOATIME) \
    Set(O_NOCTTY) \
    Set(O_NOFOLLOW) \
    Set(O_NONBLOCK) \
    Set(O_NDELAY)  \
    Set(O_PATH) \
    Set(O_SYNC) \
    Set(O_TMPFILE)  \
    Set(O_TRUNC) 
    #define Set(val) setObjectTemplateValue(isolate, flag, #val, Number::New(isolate, val));
        FLAG_LIST(Set)
    #undef Set
  #undef FLAG_LIST

  setObjectTemplateValue(isolate, constant, "MODE", mode);
  setObjectTemplateValue(isolate, constant, "FLAG", flag);
  setObjectTemplateValue(isolate, target, "constant", constant);
}
void No::FS::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> fs = ObjectTemplate::New(isolate);
  setMethod(isolate, fs, "open", No::FS::Open);
  setMethod(isolate, fs, "realpath", No::FS::Realpath);
  setMethod(isolate, fs, "openat", No::FS::OpenAt);
  setMethod(isolate, fs, "close", No::IO::Close);
  setMethod(isolate, fs, "read", No::IO::Read);
  setMethod(isolate, fs, "write", No::IO::Write);
  setMethod(isolate, fs, "readv", No::IO::ReadV);
  setMethod(isolate, fs, "writev", No::IO::WriteV);
  setMethod(isolate, fs, "dup", No::FS::Dup);
  setMethod(isolate, fs, "dup2", No::FS::Dup2);
  InitConstant(isolate, fs);
  setObjectValue(isolate, target, "fs", fs->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}



