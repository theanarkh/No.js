
#include "env.h"

class Environment;

No::Env::Environment::Environment(Local<Context> context) {
    Isolate* isolate = context->GetIsolate();
    _context.Reset(isolate, context);
    _isolate = isolate;
    io_uring_data = (struct io_uring_info *)malloc(sizeof(struct io_uring_info));
    memset(io_uring_data, 0, sizeof(*io_uring_data));
    io_uring_queue_init(1, &io_uring_data->ring, 0);
    context->SetAlignedPointerInEmbedderData(No::Env::CONTEXT_INDEX, this);
}

No::Env::Environment* No::Env::Environment::GetEnvByContext(Local<Context> context) {
    return (Environment*)context->GetAlignedPointerFromEmbedderData(No::Env::CONTEXT_INDEX);
}