#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "include/v8/libplatform/libplatform.h"
#include "include/v8/v8.h"
#include "core/core.h"
#include "core/util.h"


using namespace v8;
using namespace No::Util;
using namespace No::Core;
using namespace No::Env;

int main(int argc, char* argv[]) {
  setvbuf(stdout, nullptr, _IONBF, 0);
  setvbuf(stderr, nullptr, _IONBF, 0);
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<Platform> platform = platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
  Isolate* isolate = Isolate::New(create_params);
  {
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
    Local<Context> context = Context::New(isolate, nullptr, global);
    Context::Scope context_scope(context);
    Local<Object> No = Object::New(isolate);
    register_builtins(isolate, No);
    Local<Object> globalInstance = context->Global();
    globalInstance->Set(context, String::NewFromUtf8Literal(isolate, "No", 
    NewStringType::kNormal), No);
    // 设置全局属性global指向全局对象
    globalInstance->Set(context, String::NewFromUtf8Literal(isolate, 
      "global", 
      NewStringType::kNormal), globalInstance).Check();
    {
      // 打开文件
      int fd = open("No.js", 0, O_RDONLY);
      struct stat info;
      // 取得文件信息
      fstat(fd, &info);
      // 分配内存保存文件内容
      char *ptr = (char *)malloc(info.st_size + 1);
      // ptr[info.st_size] = '\0';
      read(fd, (void *)ptr, info.st_size);
      // 要执行的js代码
      Local<String> source = String::NewFromUtf8(isolate, ptr,
                          NewStringType::kNormal,
                          info.st_size).ToLocalChecked();

      // 编译
      Local<Script> script = Script::Compile(context, source).ToLocalChecked();
      // 解析完应该没用了，释放内存
      free(ptr);
      // 执行
      Local<Value> result = script->Run(context).ToLocalChecked();
      Run(env->GetIOUringData());
    }
  }

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::ShutdownPlatform();
  delete create_params.array_buffer_allocator;
  return 0;
}
