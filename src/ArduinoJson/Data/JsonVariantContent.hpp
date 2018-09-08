// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stdlib.h>  // size_t

#include "../Memory/AllocableInMemoryPool.hpp"
#include "JsonFloat.hpp"
#include "JsonInteger.hpp"

namespace ArduinoJson {
namespace Internals {
// Forward declarations
struct JsonObjectData;

struct JsonArrayData : AllocableInMemoryPool {
  struct Slot* head;
  struct Slot* tail;

  JsonArrayData() : head(0), tail(0) {}
};

struct RawData {
  const char* data;
  size_t size;
};

// A union that defines the actual content of a JsonVariant.
// The enum JsonVariantType determines which member is in use.
union JsonVariantContent {
  JsonFloat asFloat;
  JsonUInt asInteger;
  JsonArrayData* asArray;
  JsonObjectData* asObject;
  const char* asString;
  struct {
    const char* data;
    size_t size;
  } asRaw;
};

}  // namespace Internals
}  // namespace ArduinoJson
