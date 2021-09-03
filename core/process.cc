#include "process.h"

void No::Process::GetEnv(V8_ARGS) {
    V8_ISOLATE
    String::Utf8Value key(isolate, args[0]);
    char * value = getenv(*key);
    if (value == nullptr) {
        return V8_RETURN(Local<Value>());
    }
    Log(value);
    Local<String> str = String::NewFromUtf8(isolate, value, NewStringType::kNormal, strlen(value)).ToLocalChecked();
    V8_RETURN(str);
}

void No::Process::Fork(V8_ARGS) {
    V8_ISOLATE
    V8_RETURN(Number::New(isolate, fork()));
}

void No::Process::Wait(V8_ARGS) {
    int status;
    wait(&status);
}

void No::Process::Execve(V8_ARGS) {
    V8_ISOLATE
    int length = args.Length();
    char** ptr = new char*[length + 1];
    int i = 0;
    for (i = 0; i < length; i++) {
        String::Utf8Value arg(isolate, args[i]);
        ptr[i] = strdup(*arg);
    }
    ptr[i] = NULL;
    char *env[] = { "isMaster=0", NULL };
    // int fd[2];
    // socketpair(AF_UNIX, SOCK_STREAM, 0, fd);

    int pid = fork();
    if (pid == 0) {
        // close(fd[0]);
        execve(ptr[0], ptr, env);
        write(1, strerror(errno), sizeof(strerror(errno)));
        exit(-1);
    }
    // close(fd[1]);
    if (ptr) {
        for (int i = 0; i < length && ptr[i]; i++) {
            free(ptr[i]);
        }
        delete [] ptr;
    }
}

void No::Process::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> process = ObjectTemplate::New(isolate);
  setMethod(isolate, process, "getEnv", No::Process::GetEnv);
  setMethod(isolate, process, "fork", No::Process::Fork);
  setMethod(isolate, process, "wait", No::Process::Wait);
  setMethod(isolate, process, "execve", No::Process::Execve);
  setObjectValue(isolate, target, "process", process->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}
