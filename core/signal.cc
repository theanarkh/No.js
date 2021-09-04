
#include "signal.h"

static void signalHandler(int signum)
{   
    auto vec = signalMap.find(signum);
    if (vec != signalMap.end()) {
        vector<shared_ptr<RequestContext>>::iterator it;
        for(it=vec->second.begin();it!=vec->second.end(); it++)
        {
            makeCallback<onsignal>((*it).get());
        }
    }
}

void No::Signal::RegisterSignal(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
    Local<Object> obj = Object::New(isolate);
    Local<String> key = newStringToLcal(isolate, onsignal);
    obj->Set(context, key, args[1].As<Function>());
    shared_ptr<RequestContext> ctx = make_shared<RequestContext>(env, obj);
    int sig = args[0].As<Integer>()->Value(); 
    auto ret = signalMap.find(sig);
    if (ret == signalMap.end()) {
        signal(sig, signalHandler);
        vector<shared_ptr<RequestContext>> vec;
        vec.push_back(ctx);
        signalMap.insert(map<int, vector<shared_ptr<RequestContext>>>::value_type (sig, vec));  
        return;
    }
    ret->second.push_back(ctx);
}

void No::Signal::UnRegisterSignal(V8_ARGS) {
    int signal = args[0].As<Integer>()->Value(); 
    signalMap.erase(signal);
}

void No::Signal::Kill(V8_ARGS) {
    V8_ISOLATE
    int pid = args[0].As<Integer>()->Value(); 
    int sig = args[1].As<Integer>()->Value(); 
    V8_RETURN(Integer::New(isolate, kill(pid, sig)))
}

void No::Signal::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> signalTempalte = ObjectTemplate::New(isolate);
  setMethod(isolate, signalTempalte, "on", No::Signal::RegisterSignal);
  setMethod(isolate, signalTempalte, "off", No::Signal::UnRegisterSignal);
  setMethod(isolate, signalTempalte, "kill", No::Signal::Kill);


  Local<ObjectTemplate> constant = ObjectTemplate::New(isolate);
  Local<ObjectTemplate> SIG = ObjectTemplate::New(isolate);
  #define SIG_LIST(Set) \
    Set(SIGHUP) \
    Set(SIGINT) \
    Set(SIGQUIT) \
    Set(SIGILL) \
    Set(SIGTRAP) \
    Set(SIGABRT) \
    Set(SIGBUS) \
    Set(SIGFPE) \
    Set(SIGKILL) \
    Set(SIGUSR1) \
    Set(SIGSEGV) \
    Set(SIGUSR2) \
    Set(SIGPIPE) \
    Set(SIGALRM) \
    Set(SIGTERM) \
    Set(SIGSTKFLT) \
    Set(SIGCHLD) \
    Set(SIGCONT) \
    Set(SIGSTOP) \
    Set(SIGTSTP) \
    Set(SIGTTIN) \
    Set(SIGTTOU) \
    Set(SIGURG) \
    Set(SIGXCPU) \
    Set(SIGXFSZ) \
    Set(SIGVTALRM) \
    Set(SIGPROF) \
    Set(SIGWINCH) \
    Set(SIGIO) \
    Set(SIGPWR) \
    Set(SIGSYS) \
    
    #define Set(val) setObjectTemplateValue(isolate, SIG, #val, Number::New(isolate, val));
        SIG_LIST(Set)
    #undef Set
   
  #undef SIG_LIST
  

  setObjectTemplateValue(isolate, constant, "SIG", SIG);

  setObjectTemplateValue(isolate, signalTempalte, "constant", constant);

  setObjectValue(isolate, target, "signal", signalTempalte->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

