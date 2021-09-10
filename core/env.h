#ifndef ENV_H
#define ENV_H

#include <fcntl.h>
#include <unistd.h>
#include "include/common.h"
#include "io_uring.h"
#include "util.h"

using namespace v8;
using namespace std;
using namespace No::Util;

namespace No {
    namespace Env {
        enum {
            CONTEXT_INDEX
        } ENV_INDEX;
        
        class Environment {
            public:
                Environment(Local<Context> context);
                static Environment * GetEnvByContext(Local<Context> context);
                struct io_uring_info * GetIOUringData() {
                    return io_uring_data;
                }
                Isolate * GetIsolate() const {
                    return _isolate;
                }
                Local<Context> GetContext() const {
                    return PersistentToLocal::Strong(_context);
                }
                void setInotifyFd(int fd) {
                    inotify_fd = fd;
                }
                int getInotifyFd() {
                    return inotify_fd;
                }
            private:
                struct io_uring_info *io_uring_data;
                Global<Context> _context;
                Isolate * _isolate;
                int inotify_fd;
        };

        struct RequestContext {
            RequestContext(Environment * passEnv, Local<Object> _object)
            : env(passEnv),  object(passEnv->GetIsolate(), _object) {
                
            }
            ~RequestContext() {
                if (!object.IsEmpty()) {
                    object.Reset();
                }
                printf("finish");
            }
            Environment * env;
            Global<Object> object;
        };

        struct SignalRequestContext: public RequestContext
        {
            SignalRequestContext(Environment * passEnv, Local<Object> _object, int _sig): 
                RequestContext(passEnv, _object), sig(_sig) {}
            int sig;
        };

        struct TimerRequestContext: public RequestContext
        {
            enum {
                INTERVAL = 1
            };
            TimerRequestContext(Environment * passEnv, Local<Object> _object, int _flags, timer_t _timer_id): 
                RequestContext(passEnv, _object), flags(_flags), timer_id(_timer_id) {}
            int flags;
            timer_t timer_id;
        };
        struct InotifyRequestContext: public RequestContext
        {
            InotifyRequestContext(Environment * passEnv, Local<Object> _object, int _id): 
                RequestContext(passEnv, _object), id(_id){
                    
                }
            int id;
        };
    }
}

#endif

