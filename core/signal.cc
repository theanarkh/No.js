
#include "signal.h"

void signal_cb(void * req) {
    // to do: the SignalRequestContext object may be release before here
    struct signal_request * signal_req = (struct signal_request *)req; 
    SignalRequestContext * ctx = (SignalRequestContext *)signal_req->data;
    makeCallback<onsignal>(ctx);
    free(signal_req);
}

static void signalHandler(int signum)
{   
    auto vec = signalMap.find(signum);
    if (vec != signalMap.end()) {
        vector<shared_ptr<SignalRequestContext>>::iterator it;
        for(it=vec->second.begin();it!=vec->second.end(); it++)
        {
            struct signal_request * req = (struct signal_request *)malloc(sizeof(*req)); 
            memset(req, 0, sizeof(*req));
            req->cb = signal_cb;
            req->data = (void *)(*it).get();
            req->op = IORING_OP_NOP;
            SubmitRequest((struct request *)req, (*it).get()->env->GetIOUringData());
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
    int sig = args[0].As<Integer>()->Value(); 
    shared_ptr<SignalRequestContext> ctx = make_shared<SignalRequestContext>(env, obj, sig);
    auto ret = signalMap.find(sig);
    if (ret == signalMap.end()) {
        signal(sig, signalHandler);
        vector<shared_ptr<SignalRequestContext>> vec;
        vec.push_back(ctx);
        signalMap.insert(map<int, vector<shared_ptr<SignalRequestContext>>>::value_type (sig, vec));  
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

