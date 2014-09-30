#define MAPPO
#include <node.h>
#include "map.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Map::Init(exports);
}

NODE_MODULE(mappo, InitAll)