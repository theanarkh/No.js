#ifndef ENV_H
#define ENV_H

#include <fcntl.h>
#include <unistd.h>
#include "include/common.h"
#include "io_uring.h"
#include "util.h"

using namespace v8;
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
            private:
                struct io_uring_info *io_uring_data;
                Global<Context> _context;
                Isolate * _isolate;
        };

        struct RequestContext {
            RequestContext(Environment * passEnv, Local<Object> _object)
            : env(passEnv),  callback(env->GetIsolate(), _object) {}
            ~RequestContext() {
                if (!callback.IsEmpty()) {
                    callback.Reset();
                }
            }
            Environment * env;
            Global<Object> callback;
        };
    }
}

#endif

