#include <rapidjson/document.h>
#include <rapidjson/writer.h>

double limitNumber(double v) {
  if (std::abs(v) < 1e-7) {
    return 0;
  } else if (v > 1e9) {
    return 1e9;
  } else if (v < -1e9) {
    return -1e9;
  } else {
    return v;
  }
}

int64_t limitNumber(int64_t v) {
  if (v == INT64_MIN) {
    return v + 1;
  } else if (v == INT32_MIN) {
    return v + 1;
  } else {
    return v;
  }
}

uint64_t limitNumber(uint64_t v) {
  return v;
}

void parse_msg_rapidjson(const google::protobuf::Message& msg,
                         bool print_empty_arr,
                         rapidjson::Value& json_doc,
                         rapidjson::Document::AllocatorType& json_alloc);

void parse_repeated_field_rapidjson(const google::protobuf::Message& msg,
                                    const google::protobuf::Reflection* ref,
                                    const google::protobuf::FieldDescriptor* field,
                                    bool print_empty_arr,
                                    rapidjson::Value& json_value_array,
                                    rapidjson::Document::AllocatorType& json_alloc);

std::string pb2json(const google::protobuf::Message* msg, bool print_empty_arr) {
  rapidjson::Document json_doc;
  json_doc.SetObject();
  parse_msg_rapidjson(*msg, print_empty_arr, json_doc, json_doc.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  json_doc.Accept(writer);
  return buffer.GetString();
}

void parse_msg_rapidjson(const google::protobuf::Message& msg,
                         bool print_empty_arr,
                         rapidjson::Value& json_doc,
                         rapidjson::Document::AllocatorType& json_alloc) {
  const google::protobuf::Descriptor* d = msg.GetDescriptor();
  if (!d) {
    return;
  }

  size_t count = d->field_count();
  for (size_t i = 0; i != count; ++i) {
    const google::protobuf::FieldDescriptor* field = d->field(i);
    if (!field) {
      continue;
    }
    const google::protobuf::Reflection* ref = msg.GetReflection();
    if (!ref) {
      continue;
    }
    rapidjson::Value::StringRefType json_key(field->name().c_str(), field->name().length());

    if (field->is_repeated()) {
      const int count = ref->FieldSize(msg, field);
      if (count > 0 || print_empty_arr) {
        rapidjson::Value json_value(rapidjson::kArrayType);
        parse_repeated_field_rapidjson(msg, ref, field, print_empty_arr, json_value, json_alloc);
        json_doc.AddMember(json_key, json_value, json_alloc);
      }
    } else if (ref->HasField(msg, field)) {
      double value_double;
      int64_t value_int64;
      uint64_t value_uint64;
      bool value_bool;
      std::string value_string;
      rapidjson::Value json_value_string(rapidjson::kStringType);
      rapidjson::Value json_value_object(rapidjson::kObjectType);

      switch (field->cpp_type()) {
        case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
          value_double = ref->GetDouble(msg, field);
          json_doc.AddMember(json_key, limitNumber(value_double), json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
          value_double = ref->GetFloat(msg, field);
          json_doc.AddMember(json_key, limitNumber(value_double), json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
          value_int64 = ref->GetInt64(msg, field);
          json_doc.AddMember(json_key, limitNumber(value_int64), json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
          value_uint64 = ref->GetUInt64(msg, field);
          json_doc.AddMember(json_key, limitNumber(value_uint64), json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
          value_int64 = ref->GetInt32(msg, field);
          json_doc.AddMember(json_key, limitNumber(value_int64), json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
          value_uint64 = ref->GetUInt32(msg, field);
          json_doc.AddMember(json_key, limitNumber(value_uint64), json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
          value_int64 = ref->GetEnum(msg, field)->number();
          json_doc.AddMember(json_key, limitNumber(value_int64), json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
          value_bool = ref->GetBool(msg, field);
          json_doc.AddMember(json_key, value_bool, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
          value_string = ref->GetString(msg, field);
          json_value_string.SetString(value_string.c_str(), value_string.length(), json_alloc);
          json_doc.AddMember(json_key, json_value_string, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
          parse_msg_rapidjson(ref->GetMessage(msg, field), print_empty_arr, json_value_object, json_alloc);
          json_doc.AddMember(json_key, json_value_object, json_alloc);
          break;
        default:
          break;
      }
    }
  }
}

void parse_repeated_field_rapidjson(const google::protobuf::Message& msg,
                                    const google::protobuf::Reflection* ref,
                                    const google::protobuf::FieldDescriptor* field,
                                    bool print_empty_arr,
                                    rapidjson::Value& json_value_array,
                                    rapidjson::Document::AllocatorType& json_alloc) {
  const int count = ref->FieldSize(msg, field);
  if (count <= 0) {
    return;
  }
  switch (field->cpp_type()) {
    case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
      for (int i = 0; i < count; i++) {
        double value = ref->GetRepeatedDouble(msg, field, i);
        json_value_array.PushBack(limitNumber(value), json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
      for (int i = 0; i < count; i++) {
        double value = ref->GetRepeatedFloat(msg, field, i);
        json_value_array.PushBack(limitNumber(value), json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
      for (int i = 0; i < count; i++) {
        int64_t value = ref->GetRepeatedInt64(msg, field, i);
        json_value_array.PushBack(limitNumber(value), json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
      for (int i = 0; i < count; i++) {
        uint64_t value = ref->GetRepeatedUInt64(msg, field, i);
        json_value_array.PushBack(limitNumber(value), json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
      for (int i = 0; i < count; i++) {
        int64_t value = ref->GetRepeatedInt32(msg, field, i);
        json_value_array.PushBack(limitNumber(value), json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
      for (int i = 0; i < count; i++) {
        uint64_t value = ref->GetRepeatedUInt32(msg, field, i);
        json_value_array.PushBack(limitNumber(value), json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
      for (int i = 0; i < count; i++) {
        int64_t value = ref->GetRepeatedEnum(msg, field, i)->number();
        json_value_array.PushBack(limitNumber(value), json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
      for (int i = 0; i < count; i++) {
        bool value = ref->GetRepeatedBool(msg, field, i);
        json_value_array.PushBack(value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
      for (int i = 0; i < count; i++) {
        std::string value = ref->GetRepeatedString(msg, field, i);
        rapidjson::Value json_value(rapidjson::kStringType);
        json_value.SetString(value.c_str(), value.length(), json_alloc);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
      for (int i = 0; i < count; i++) {
        auto& value = ref->GetRepeatedMessage(msg, field, i);
        rapidjson::Value json_value(rapidjson::kObjectType);
        parse_msg_rapidjson(value, print_empty_arr, json_value, json_alloc);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    default:
      break;
  }
}
