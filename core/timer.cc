
#include "timer.h"

static void signalHandler(int sig, siginfo_t *si, void *uc)
{   
    timer_t *timer_id = (timer_t *)si->si_value.sival_ptr;
    // int or = timer_getoverrun(*tidp);
    auto obj = timerMap.find(*timer_id);
    if (obj != timerMap.end()) {
        shared_ptr<TimerRequestContext> ctx = obj->second;
        makeCallback<ontimeout>(ctx.get());
        if (!(ctx->flags & TimerRequestContext::INTERVAL)) {
            timerMap.erase(*timer_id);
            free(timer_id);
        }
    }
}

static void initTimer() {
    struct sigaction act;  
    memset(&act, 0, sizeof(act));  
    act.sa_sigaction = signalHandler;  
    act.sa_flags = SA_SIGINFO;  

    sigemptyset(&act.sa_mask);  

    if (sigaction(SIGUSR2, &act, NULL) == -1) {
        perror("fail to sigaction");
        return;
    }
}

static u_int64_t timerHandler(V8_ARGS) {
    V8_ISOLATE
    int timeout = args[1].As<Integer>()->Value(); 
    int interval = 0;
    if (args.Length() > 2) {
        interval = args[2].As<Integer>()->Value(); 
    }
    timer_t * timerid = (timer_t *)malloc(sizeof(timer_t));  
    memset(timerid, 0, sizeof(*timerid));
    struct sigevent evp;  
    memset(&evp, 0, sizeof(struct sigevent));  
    evp.sigev_signo = SIGUSR2;  
    evp.sigev_notify = SIGEV_SIGNAL;  
    evp.sigev_value.sival_ptr = timerid;  
    if (timer_create(CLOCK_REALTIME, &evp, timerid) == -1)  
    {  
        perror("fail to timer_create");  
        return -1;
    }  
    int second = timeout / 1000;
    int nsecond = (timeout % 1000) * 1000 * 1000;
    int interval_second = interval / 1000;
    int interval_nsecond = (interval % 1000) * 1000 * 1000;
    struct itimerspec it;  
    it.it_interval.tv_sec = interval_second;  
    it.it_interval.tv_nsec = interval_nsecond;  
    it.it_value.tv_sec = second;  
    it.it_value.tv_nsec = nsecond;  
    if (timer_settime(*timerid, 0, &it, 0) == -1)  
    {  
        perror("fail to timer_settime");  
        return -1; 
    }  
    V8_CONTEXT
    Environment *env = Environment::GetEnvByContext(context);
    Local<Object> obj = Object::New(isolate);
    Local<String> key = newStringToLcal(isolate, ontimeout);
    obj->Set(context, key, args[0].As<Function>());
    int intervalFlags = interval_second || interval_nsecond ? TimerRequestContext::INTERVAL : 0;
    shared_ptr<TimerRequestContext> ctx = make_shared<TimerRequestContext>(env, obj, intervalFlags);
    auto ret = timerMap.find(*timerid);
    if (ret == timerMap.end()) {
        timerMap.insert(map<timer_t, shared_ptr<TimerRequestContext>>::value_type (*timerid, ctx));  
    }
    return (u_int64_t)*timerid;
}

void No::Timer::SetTimeout(V8_ARGS) {
    V8_ISOLATE
    V8_RETURN(BigInt::New(isolate, (u_int64_t)timerHandler(args)))
}

void No::Timer::SetInterval(V8_ARGS) {
    V8_ISOLATE
    V8_RETURN(BigInt::New(isolate, (u_int64_t)timerHandler(args)))
}

void No::Timer::ClearTimeout(V8_ARGS) {
    timer_t id = (timer_t)(args[0].As<BigInt>()->Uint64Value());
    timerMap.erase(id);
    timer_delete(id);
}

void No::Timer::ClearInterval(V8_ARGS) {
    timer_t id = (timer_t)(args[0].As<BigInt>()->Uint64Value());
    timerMap.erase(id);
    timer_delete(id);
}

void No::Timer::Init(Isolate* isolate, Local<Object> target) {
  initTimer();
  Local<ObjectTemplate> timer = ObjectTemplate::New(isolate);
  setMethod(isolate, timer, "setTimeout", No::Timer::SetTimeout);
  setMethod(isolate, timer, "setInterval", No::Timer::SetInterval);
  setMethod(isolate, timer, "clearTimeout", No::Timer::ClearTimeout);
  setMethod(isolate, timer, "clearInterval", No::Timer::ClearInterval);
  setObjectValue(isolate, target, "timer", timer->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

