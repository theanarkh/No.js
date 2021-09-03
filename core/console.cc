// #include "console.h"

// void No::Console::log(V8_ARGS) {
//     V8_ISOLATE
//     String::Utf8Value str(isolate, args[0]);
//     Log(*str);
// }

// void No::Console::Init(Isolate* isolate, Local<ObjectTemplate> target) {
//   setMethod(isolate, target, "log", No::Console::log);
// }

#include "console.h"
#include <stdio.h>
void No::Console::log(V8_ARGS) {
    V8_ISOLATE
    String::Utf8Value str(isolate, args[0]);
    Log(*str);
}

void No::Console::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> console = ObjectTemplate::New(isolate);
  setMethod(isolate, console, "log", No::Console::log);
  setObjectValue(isolate, target, "console", console->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

