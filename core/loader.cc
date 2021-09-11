#include "loader.h"      


void Compile(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    node::Utf8Value filename(isolate, args[0].As<String>());
    const char* id = *id_v;
    // Local<Object> ext[1];
    // Local<Object> obj = Object::New(isolate);
    // obj->Set(context, newStringToLcal(isolate ,"xx"), newStringToLcal(isolate, "yy"));
    // ext[0] = obj;
    int fd = open(*filename, 0 , O_RDONLY);
    read(fd);
    ScriptCompiler::Source script_source(newStringToLcal(isolate,"return {}"));
    Local<String> params[] = {newStringToLcal(isolate, "require")};
    Local<Function> fun =
    ScriptCompiler::CompileFunctionInContext(context, &script_source,
                                                    1, params, 0, nullptr).ToLocalChecked();

    Local<Value> arg_value[] = {Number::New(isolate, 42.0)};
    Local<Value> result = fun->Call(context, context->Global(), 2, arg_value).ToLocalChecked();
    if (!result.IsEmpty()) {
        args.GetReturnValue().Set(result.ToLocalChecked());
    }
    
}

void No::Loader::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> loader = ObjectTemplate::New(isolate);
  setMethod(isolate, loader, "compile", No::Loader::Compile);
  setObjectValue(isolate, target, "Loader", loader->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}
