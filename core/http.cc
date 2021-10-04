#include "http.h"

void No::HTTP::Parser::New(const FunctionCallbackInfo<Value>& args) {
    Environment* env = Environment::GetEnvByContext(args.GetIsolate()->GetCurrentContext());
    new Parser(env, args.This());
}

void No::HTTP::Parser::Parse(const FunctionCallbackInfo<Value>& args) {
    
}

void No::HTTP::Init(Isolate* isolate, Local<Object> target) {
    Local<FunctionTemplate> parser = FunctionTemplate::New(isolate, No::HTTP::Parser::New);
    parser->InstanceTemplate()->SetInternalFieldCount(1);
    parser->SetClassName(newString(isolate, "HTTPParser"));
    parser->PrototypeTemplate()->Set(newString(isolate, "parse"), FunctionTemplate::New(isolate, No::HTTP::Parser::Parse));
    setObjectValue(isolate, target, "HTTPParser", parser->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
}



