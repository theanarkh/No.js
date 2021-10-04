#include "baseObject.h"

No::Base::BaseObject::BaseObject(Environment* env, Local<Object> object)
    : _env(env), _object(env->GetIsolate(), object) {
    object->SetAlignedPointerInInternalField(0, static_cast<void*>(this));
}

No::Base::BaseObject::~BaseObject() {
    object()->SetAlignedPointerInInternalField(0, nullptr);

}

Environment* No::Base::BaseObject::env() const {
  return _env;
}

Local<Object> No::Base::BaseObject::object() {
  return PersistentToLocal::Strong(_object);
}

void * No::Base::BaseObject::unwrap(Local<Object> object) {
  return object->GetAlignedPointerFromInternalField(0);
}