#include "loader.h"      


void No::Loader::Compile(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    String::Utf8Value filename(isolate, args[0].As<String>());
    int fd = open(*filename, 0 , O_RDONLY);
    std::string content;
    char buffer[4096];
    while (1)
    {
      memset(buffer, 0, 4096);
      int ret = read(fd, buffer, 4096);
      if (ret == -1) {
        return args.GetReturnValue().Set(newStringToLcal(isolate, "read file error"));
      }
      if (ret == 0) {
        break;
      }
      content.append(buffer, ret);
    }
    close(fd);
    ScriptCompiler::Source script_source(newStringToLcal(isolate, content.c_str()));
    Local<String> params[] = {
      newStringToLcal(isolate, "require"),
      newStringToLcal(isolate, "exports"),
      newStringToLcal(isolate, "module"),
    };
    MaybeLocal<Function> fun =
    ScriptCompiler::CompileFunctionInContext(context, &script_source, 3, params, 0, nullptr);
    if (fun.IsEmpty()) {
      args.GetReturnValue().Set(Undefined(isolate));
    } else {
      args.GetReturnValue().Set(fun.ToLocalChecked());
    }
}

void No::Loader::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> loader = ObjectTemplate::New(isolate);
  setMethod(isolate, loader, "compile", No::Loader::Compile);
  setObjectValue(isolate, target, "loader", loader->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}
