#ifndef ASYNC_H
#define ASYNC_H

#include "util.h"
#include "env.h"
#include "io_uring.h"
#include "include/common.h"

using namespace v8;
using namespace No::Util;
using namespace No::Env;
using namespace No::io_uring;

namespace No {
    namespace Async {
        template <const char * event>
        void makeCallback(void * req) {
            struct request * _req = (struct request *)req;
            RequestContext* ctx =(RequestContext *)_req->data;
            if (!ctx->object.IsEmpty()) {
                Local<Object> object = ctx->object.Get(ctx->env->GetIsolate());
                Local<Value> cb;
                Local<Context> context = ctx->env->GetContext();
                Local<String> onevent = newStringToLcal(ctx->env->GetIsolate(), event);       
                object->Get(context, onevent).ToLocal(&cb);
                if (cb->IsFunction()) {	
                    Local<Value> argv[] = {
                        Integer::New(context->GetIsolate(), _req->res)
                    };
                    cb.As<v8::Function>()->Call(context, object, 1, argv);
                }
            }
            if (_req->flag & IO_URING_REQUEST_AGAIN) {
                struct io_uring_info *io_uring_data = ctx->env->GetIOUringData();
                SubmitRequest(_req, io_uring_data);
            } else {
                delete ctx;
                free(_req);
            }
        };
        template <const char * event>
        void makeCallback(RequestContext * ctx) {
            if (!ctx->object.IsEmpty()) {
                Local<Object> object = ctx->object.Get(ctx->env->GetIsolate());
                Local<Value> cb;
                Local<Context> context = ctx->env->GetContext();
                Local<String> onevent = newStringToLcal(ctx->env->GetIsolate(), event);       
                object->Get(context, onevent).ToLocal(&cb);
                if (cb->IsFunction()) {	
                    Local<Value> argv[] = {
                        Integer::New(context->GetIsolate(), 0)
                    };
                    cb.As<v8::Function>()->Call(context, object, 1, argv);
                }
            }
        };
    }
}

#endif