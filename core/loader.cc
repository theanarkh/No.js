#include "loader.h"      

MaybeLocal<Function> CompileAsModule(
    Local<Context> context,
    const char* id) {
  Isolate* isolate = context->GetIsolate();
  std::vector<Local<String>>* parameters = {
      newStringToLcal(isolate, "exports"),
      newStringToLcal(isolate, "require"),
      newStringToLcal(isolate, "module"),
      newStringToLcal(isolate, "process"),
      newStringToLcal(isolate, "internalBinding"),
    };

  EscapableHandleScope scope(isolate);

  Local<String> source;
  if (!LoadBuiltinModuleSource(isolate, id).ToLocal(&source)) {
    return {};
  }

  std::string filename_s = id + std::string(".js");
  Local<String> filename =
      OneByteString(isolate, filename_s.c_str(), filename_s.size());
  Local<Integer> line_offset = Integer::New(isolate, 0);
  Local<Integer> column_offset = Integer::New(isolate, 0);
  ScriptOrigin origin(filename, line_offset, column_offset, True(isolate));

  Mutex::ScopedLock lock(code_cache_mutex_);

  ScriptCompiler::CachedData* cached_data = nullptr;
  {
    auto cache_it = code_cache_.find(id);
    if (cache_it != code_cache_.end()) {
      // Transfer ownership to ScriptCompiler::Source later.
      cached_data = cache_it->second.release();
      code_cache_.erase(cache_it);
    }
  }

  const bool has_cache = cached_data != nullptr;
  ScriptCompiler::CompileOptions options =
      has_cache ? ScriptCompiler::kConsumeCodeCache
                : ScriptCompiler::kEagerCompile;
  ScriptCompiler::Source script_source(source, origin, cached_data);

  MaybeLocal<Function> maybe_fun =
      ScriptCompiler::CompileFunctionInContext(context,
                                               &script_source,
                                               parameters->size(),
                                               parameters->data(),
                                               0,
                                               nullptr,
                                               options);

  // This could fail when there are early errors in the native modules,
  // e.g. the syntax errors
  if (maybe_fun.IsEmpty()) {
    // In the case of early errors, v8 is already capable of
    // decorating the stack for us - note that we use CompileFunctionInContext
    // so there is no need to worry about wrappers.
    return MaybeLocal<Function>();
  }

  Local<Function> fun = maybe_fun.ToLocalChecked();
  // XXX(joyeecheung): this bookkeeping is not exactly accurate because
  // it only starts after the Environment is created, so the per_context.js
  // will never be in any of these two sets, but the two sets are only for
  // testing anyway.

  *result = (has_cache && !script_source.GetCachedData()->rejected)
                ? Result::kWithCache
                : Result::kWithoutCache;
  // Generate new cache for next compilation
  std::unique_ptr<ScriptCompiler::CachedData> new_cached_data(
      ScriptCompiler::CreateCodeCacheForFunction(fun));
  CHECK_NOT_NULL(new_cached_data);

  // The old entry should've been erased by now so we can just emplace
  code_cache_.emplace(id, std::move(new_cached_data));

  return scope.Escape(fun);
}

void Compile(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    node::Utf8Value id_v(isolate, args[0].As<String>());
    const char* id = *id_v;
    MaybeLocal<Function> maybe = CompileAsModule(context, id);
    if (!maybe.IsEmpty()) {
        args.GetReturnValue().Set(maybe.ToLocalChecked());
    }
}

void No::Loader::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> loader = ObjectTemplate::New(isolate);
  setMethod(isolate, loader, "compile", No::Loader::Compile);
  setObjectValue(isolate, target, "Loader", loader->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}
