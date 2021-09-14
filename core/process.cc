#include "process.h"

void No::Process::GetEnv(V8_ARGS) {
    V8_ISOLATE
    String::Utf8Value key(isolate, args[0]);
    char * value = getenv(*key);
    if (value == nullptr) {
        return V8_RETURN(Local<Value>());
    }
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

void No::Process::Exit(V8_ARGS) {
    int status = 0;
    if (args.Length() > 0) {
        status = args[0].As<Integer>()->Value();
    }
    exit(status);
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


void EnvGetter(Local<Name> name, const v8::PropertyCallbackInfo<v8::Value>& args) {
  V8_ISOLATE     
  // to do: handle the key of toString
  String::Utf8Value key(isolate, name);
  char * value = getenv(*key);
  if (value == nullptr) {
    return V8_RETURN(Local<Value>());
  }
  V8_RETURN(newStringToLcal(isolate, value));      
}

static void EnvSetter(Local<Name> key,
                      Local<Value> value,
                      const PropertyCallbackInfo<Value>& args) {                                                  
  V8_ISOLATE
  V8_CONTEXT
  Local<String> key_strng = key.As<String>();
  Local<String> value_string = value.As<String>();;
  if (key.IsEmpty() || value.IsEmpty()) {
    return;
  }

  {
    String::Utf8Value key(isolate, key_strng);
    String::Utf8Value value(isolate, value_string);
    setenv(*key, *value, 1);
  }
}

static void EnvDeleter(Local<Name> key,
                       const PropertyCallbackInfo<Boolean>& args) {
  V8_ISOLATE                        
  Local<String> keyObject = key.As<String>();
  String::Utf8Value key_stirng(isolate, keyObject);
  unsetenv(*key_stirng);
  V8_RETURN(true)
}

static void EnvEnumerator(const PropertyCallbackInfo<Array>& args) {
    // to do
}

void No::Process::Init(Isolate* isolate, Local<Object> target) {
  Local<Context> context = isolate->GetCurrentContext();
  Environment * env = Environment::GetEnvByContext(context);
  Local<ObjectTemplate> process = ObjectTemplate::New(isolate);
  setMethod(isolate, process, "getEnv", No::Process::GetEnv);
  setMethod(isolate, process, "fork", No::Process::Fork);
  setMethod(isolate, process, "wait", No::Process::Wait);
  setMethod(isolate, process, "exit", No::Process::Exit);
  setMethod(isolate, process, "execve", No::Process::Execve);
  char ** argv = env->getArgv();
  int argc = env->getArgc();
  Local<Array> arr = Array::New(isolate, argc);
  for (int i = 0; i < argc; i++) {
      arr->Set(context, Number::New(isolate, i), newStringToLcal(isolate, argv[i]));
  }
  
  Local<ObjectTemplate> env_proxy_template = ObjectTemplate::New(isolate);
  Local<Object> env_obj = Object::New(isolate);
  // env_obj->Set(context, newStringToLcal(isolate, "x"), newStringToLcal(isolate, "y"));
  env_proxy_template->SetHandler(NamedPropertyHandlerConfiguration(
      EnvGetter, EnvSetter, nullptr, EnvDeleter, EnvEnumerator, Local<Value>(),
      PropertyHandlerFlags::kHasNoSideEffect));
  
  Local<Object> obj = process->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
  obj->Set(context, newStringToLcal(isolate, "env"), env_proxy_template->NewInstance(context).ToLocalChecked());
  setObjectValue(isolate, obj, "argv", arr);
  setObjectValue(isolate, target, "process", obj);
}
