#include "baseObject.h"

No::Base::BaseObject(Environment* env, Local<Object> object)
    : _env(env) {
    object->SetAlignedPointerInInternalField(0, static_cast<void*>(this));
}

Environment* No::Base::BaseObject::env() const {
  return _env;
}

Local<Object> No::Base::BaseObject::object() const {
  return object->GetAlignedPointerFromInternalField(0);
}