#define MAPPO
#include <node.h>
#include "map.h"

using namespace v8;

Persistent<Function> Map::constructor;

Map::Map(int widht, int height) {
}

Map::~Map() {
}

void Map::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Map"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("describe"),
      FunctionTemplate::New(Describe)->GetFunction());
  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Map"), constructor);
}

Handle<Value> Map::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Map(...)`
    int width = args[0]->IsUndefined() ? 0 : args[0]->IntegerValue();
    int height = args[1]->IsUndefined() ? 0 : args[1]->IntegerValue();
    Map* obj = new Map(width, height);
    obj->Wrap(args.This());
    return args.This();
  } else {
    // Invoked as plain function `Map(...)`, turn into construct call.
    const int argc = 2;
    Local<Value> argv[argc] = { args[0], args[1] };
    return scope.Close(constructor->NewInstance(argc, argv));
  }
}

Handle<Value> Map::Describe(const Arguments& args) {
  HandleScope scope;

  //MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
  //obj->value_ += 1;

  return scope.Close(String::New("Hello!"));
}