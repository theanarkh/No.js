#ifndef HTTP_H
#define HTTP_H

#include "HTTP_Parser.h"
#include "util.h"
#include "include/common.h"
#include "io_uring.h"
#include "env.h"
#include "baseObject.h"
#include "../include/llhttp/llhttp.h"

using namespace v8;
using namespace No::Util;
using namespace No::Env;
using namespace No::io_uring;
using namespace No::Base;
using namespace No::HTTP;

namespace No {
    namespace HTTP {
        class Parser : public BaseObject {
            public:
                Parser(Environment* env, Local<Object> object): BaseObject(env, object) {
                    parser_callback callback = {
                        this,
                        [](on_headers_complete_info info, parser_callback callback) {
                            Parser * parser = (Parser *)callback.data;
                            Local<Value> cb;
                            Local<Context> context = parser->env()->GetContext();
                            Isolate * isolate = parser->env()->GetIsolate();
                            Local <String> key = newStringToLcal(isolate, "onHeaderComplete");
                            parser->object()->Get(context, key).ToLocal(&cb);
                            if (!cb.IsEmpty() && cb->IsFunction()) {
                                Local<Object> obj = Object::New(isolate);
                                obj->Set(context, newStringToLcal(isolate, "major_version"), Integer::New(isolate, info.major_version));
                                obj->Set(context, newStringToLcal(isolate, "minor_version"), Integer::New(isolate, info.minor_version));
                                obj->Set(context, newStringToLcal(isolate, "keepalive"), Integer::New(isolate, info.keepalive));
                                obj->Set(context, newStringToLcal(isolate, "upgrade"), Integer::New(isolate, info.upgrade));
                                obj->Set(context, newStringToLcal(isolate, "url"), newStringToLcal(isolate, info.url.c_str()));
                                Local<Object> headers = Object::New(isolate);
                                for (int i = 0; i < info.keys.size(); i++) {
                                    headers->Set(context, newStringToLcal(isolate, info.keys[i].c_str()), newStringToLcal(isolate, info.values[i].c_str()));
                                }
                                obj->Set(context, newStringToLcal(isolate, "headers"), headers);
                                Local<Value> argv[] = {
                                    obj
                                };
                                cb.As<v8::Function>()->Call(context, parser->object(), 1, argv);  
                            }
                        },

                        [](on_body_info info, parser_callback callback) {
                            Parser * parser = (Parser *)callback.data;
                            Local<Value> cb;
                            Local<Context> context = parser->env()->GetContext();
                            Isolate * isolate = parser->env()->GetIsolate();
                            Local <String> key = newStringToLcal(isolate, "onBody");
                            parser->object()->Get(context, key).ToLocal(&cb);
                            if (!cb.IsEmpty() && cb->IsFunction()) {
                                Local<Value> argv[] = {
                                    newStringToLcal(isolate, info.body.c_str())
                                };
                                cb.As<v8::Function>()->Call(context, parser->object(), 1, argv);  
                            }
                        },

                        [](on_body_complete_info info, parser_callback callback) {
                            Parser * parser = (Parser *)callback.data;
                            Local<Value> cb;
                            Local<Context> context = parser->env()->GetContext();
                            Isolate * isolate = parser->env()->GetIsolate();
                            Local <String> key = newStringToLcal(isolate, "onBodyComplete");
                            parser->object()->Get(context, key).ToLocal(&cb);
                            if (!cb.IsEmpty() && cb->IsFunction()) {
                                Local<Value> argv[] = {
                                    newStringToLcal(isolate, info.body.c_str())
                                };
                                cb.As<v8::Function>()->Call(context, parser->object(), 1, argv);  
                            }
                        },
                    };
                    httpparser = new HTTP_Parser(HTTP_REQUEST, callback);
                }
                ~Parser() {
                    if (httpparser) {
                        delete httpparser;
                    }
                }
                void Parse(const char * data, size_t len); 
                void test() {
                    printf("hello");
                }
                static void Parse(const FunctionCallbackInfo<Value>& args); 
                static void New(const FunctionCallbackInfo<Value>& args); 
            private:
                HTTP_Parser * httpparser;
        };
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif