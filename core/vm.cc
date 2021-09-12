#include "vm.h"      

void No::VM::Run(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    String::Utf8Value str(isolate, args[0].As<String>());
    Local<Object> options = args[1].As<Object>();
    Local<Value> mode;
    options->Get(context, newStringToLcal(isolate, "mode")).ToLocal(&mode);
    MODE code_mode = (MODE)mode.As<Integer>()->Value();
    std::string code;
    if (code_mode == FILE_MODE) {
      int fd = open(*str, 0, O_RDONLY);
      if (fd == -1) {
        return V8_RETURN(newStringToLcal(isolate, "open file error"));
      }
      char buffer[4096];
      while (1)
      {
        memset(buffer, 0, 4096);
        int ret = read(fd, buffer, 4096);
        if (ret == -1) {
          return V8_RETURN(newStringToLcal(isolate, "read file error"));
        }
        if (ret == 0) {
          break;
        }
        code.append(buffer, ret);
      }
      close(fd);
    } else {
      code.append(*str, str.length());
    }

    Local<String> source = String::NewFromUtf8(isolate, code.c_str(),
                          NewStringType::kNormal,
                          code.length()).ToLocalChecked();

    // 编译
    MaybeLocal<Script> script = Script::Compile(context, source);
    if (script.IsEmpty()) {
      return V8_RETURN(newStringToLcal(isolate, "invalid code"));
    }
    // 执行
    Local<Value> result = script.ToLocalChecked()->Run(context).ToLocalChecked();
    V8_RETURN(result)
}

void No::VM::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> vm = ObjectTemplate::New(isolate);

  {
    Local<ObjectTemplate> mode = ObjectTemplate::New(isolate);
    setObjectTemplateValue(isolate, mode, "CODE", Integer::New(isolate, CODE_MODE));
    setObjectTemplateValue(isolate, mode, "FILE", Integer::New(isolate, FILE_MODE));
    setObjectTemplateValue(isolate, vm, "MODE", mode);
  }
  
  setMethod(isolate, vm, "run", No::VM::Run);
  setObjectValue(isolate, target, "vm", vm->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}
