#define MAPPO
#include <node.h>
#include <node_buffer.h>
#include <vector>
#include "map.h"
#include "mappo/map.h"

using namespace v8;

Persistent<Function> Map::constructor;

Map::Map(int width, int height): map(width, height) {
}

Map::~Map() {
}

void Map::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Map"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("clear"),
      FunctionTemplate::New(Clear)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getPNG"),
      FunctionTemplate::New(GetPNG)->GetFunction());
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

Handle<Value> Map::Clear(const Arguments& args) {
  HandleScope scope;

  int red = args[0]->IntegerValue();
  int green = args[1]->IntegerValue();
  int blue = args[2]->IntegerValue();

  Map* obj = ObjectWrap::Unwrap<Map>(args.This());
  obj->map.clear(red, green, blue);
  return scope.Close(Undefined());
}

Handle<Value> Map::GetPNG(const Arguments& args) {
  HandleScope scope;

  Map* obj = ObjectWrap::Unwrap<Map>(args.This());
  std::vector<unsigned char>* png = obj->map.writePNG();
  v8::Handle<v8::Object> buffer = node::Buffer::New(reinterpret_cast<char*>(png->data()), png->size())->handle_;
  return scope.Close(buffer);
}