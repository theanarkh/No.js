#include "signal.h"

void signalHandler(int signum)
{
    auto ret = No::Signale::signalMap.find(signal);
    if (ret != No::Signale::signalMap.end()) {
        ret->second(signum);
    }
}

void No::Signal::RegisterSignal(V8_ARGS) {
    V8_ISOLATE
    int signal = args[0].As<Integer>()->Value(); 
    Local<Function> cb = args[1].As<Function>(); 
    if (!No::Signale::signalMap.find(signal)) {
        signal(signal, signalHandler);
    }
    Global<Object> callback(isolate, cb);
    No::Signale::signalMap[signal] = callback;
}

void No::Signal::UnRegisterSignal(V8_ARGS) {
    int signal = args[0].As<Integer>()->Value(); 
    No::Signale::signalMap.erase(signal);
}

void No::Signal::Kill(V8_ARGS) {
    V8_ISOLATE
    int pid = args[0].As<Integer>()->Value(); 
    int sig = args[1].As<Integer>()->Value(); 
    V8_RETURN(Interge::New(isolate, kill(pid, sig)))
}

void No::Signal::Init(Isolate* isolate, Local<ObjectTemplate> target) {
  Local<ObjectTemplate> signal = ObjectTemplate::New(isolate);
  setMethod(isolate, signal, "registerSignal", No::Signal::RegisterSignal);
  setMethod(isolate, signal, "unregisterSignal", No::Signal::UnRegisterSignal);
  setMethod(isolate, signal, "kill", No::Signal::Kill);
  setTemplate(isolate, target, "signal", signal);
}