#ifndef MAP_H
#define MAP_H

#include <node.h>

class Map : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit Map(int width = 0, int height = 0);
  ~Map();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Describe(const v8::Arguments& args);
  static v8::Persistent<v8::Function> constructor;
};

#endif