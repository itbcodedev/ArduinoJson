// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonObjectData.hpp"
#include "JsonVariant.hpp"
#include "Numbers/parseFloat.hpp"
#include "Numbers/parseInteger.hpp"

#include <string.h>  // for strcmp

namespace ArduinoJson {

inline bool JsonVariant::set(const JsonArray& array) {
  if (!_data) return false;
  if (array._data)
    _data->setArray(*array._data);
  else
    _data->setNull();
  return true;
}

inline bool JsonVariant::set(const Internals::JsonArraySubscript& value) {
  return set(value.as<JsonVariant>());
}

inline bool JsonVariant::set(const JsonObject& object) {
  if (!_data) return false;
  if (object._data)
    _data->setObject(*object._data);
  else
    _data->setNull();
  return true;
}

template <typename TString>
inline bool JsonVariant::set(
    const Internals::JsonObjectSubscript<TString>& value) {
  return set(value.template as<JsonVariant>());
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonArray>::value,
    JsonArray>::type
JsonVariant::as() const {
  return _data ? JsonArray(_memoryPool, _data->asArray()) : JsonArray();
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonObject>::value,
    T>::type
JsonVariant::as() const {
  return _data ? JsonObject(_memoryPool, _data->asObject()) : JsonObject();
}

inline JsonArray JsonVariant::toArray() {
  JsonArray array(_memoryPool);
  set(array);
  return array;
}

inline JsonObject JsonVariant::toObject() {
  JsonObject obj(_memoryPool);
  set(obj);
  return obj;
}

template <typename Visitor>
inline void JsonVariant::visit(Visitor& visitor) const {
  using namespace Internals;
  if (!_data) return visitor.acceptNull();

  switch (_data->type) {
    case JSON_FLOAT:
      return visitor.acceptFloat(_data->content.asFloat);

    case JSON_ARRAY:
      return visitor.acceptArray(
          JsonArray(_memoryPool, _data->content.asArray));

    case JSON_OBJECT:
      return visitor.acceptObject(
          JsonObject(_memoryPool, _data->content.asObject));

    case JSON_STRING:
      return visitor.acceptString(_data->content.asString);

    case JSON_RAW:
      return visitor.acceptRawJson(_data->content.asRaw.data,
                                   _data->content.asRaw.size);

    case JSON_NEGATIVE_INTEGER:
      return visitor.acceptNegativeInteger(_data->content.asInteger);

    case JSON_POSITIVE_INTEGER:
      return visitor.acceptPositiveInteger(_data->content.asInteger);

    case JSON_BOOLEAN:
      return visitor.acceptBoolean(_data->content.asInteger != 0);

    default:
      return visitor.acceptNull();
  }
}
}  // namespace ArduinoJson
