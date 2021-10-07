#include "http.h"

void No::HTTP::Parser::New(const FunctionCallbackInfo<Value>& args) {
    Environment* env = Environment::GetEnvByContext(args.GetIsolate()->GetCurrentContext());
    new Parser(env, args.This());
}

void No::HTTP::Parser::Parse(const char * data, size_t len) {
    // string str;
    // str.append(data, len);
    // cout<<str;
    httpparser->parse(data, len);
    // httpparser->print();
}

void No::HTTP::Parser::Parse(const FunctionCallbackInfo<Value>& args) {
    Parser * parser = (Parser *)unwrap(args.This());
    Local<Uint8Array> uint8Array = args[0].As<Uint8Array>();
    Local<ArrayBuffer> arrayBuffer = uint8Array->Buffer();
    std::shared_ptr<BackingStore> backing = arrayBuffer->GetBackingStore();
    const char * data = (const char * )backing->Data();
    parser->Parse(data, strlen(data));
}

void No::HTTP::Init(Isolate* isolate, Local<Object> target) {
    Local<FunctionTemplate> parser = FunctionTemplate::New(isolate, No::HTTP::Parser::New);
    parser->InstanceTemplate()->SetInternalFieldCount(1);
    parser->SetClassName(newStringToLcal(isolate, "HTTPParser"));
    parser->PrototypeTemplate()->Set(newStringToLcal(isolate, "parse"), FunctionTemplate::New(isolate, No::HTTP::Parser::Parse));
    setObjectValue(isolate, target, "HTTPParser", parser->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
}



