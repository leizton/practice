#include <rapidjson/document.h>
#include <rapidjson/writer.h>

/*
void parse_msg_rapidjson(const google::protobuf::Message *msg,
                         bool print_empty_arr,
                         rapidjson::Document& json_doc);
void parse_repeated_field_rapidjson(const google::protobuf::Message *msg,
                                    const google::protobuf::Reflection * ref,
                                    const google::protobuf::FieldDescriptor *field,
                                    bool print_empty_arr,
                                    rapidjson::Value& json_value_array,
                                    rapidjson::Document& json_doc);
void parse_msg_rapidjson2(const google::protobuf::Message *msg,
                          bool print_empty_arr,
                          rapidjson::Value& json_doc,
                          rapidjson::Document& json_doc_ori);

std::string pb2json(const google::protobuf::Message *msg, bool print_empty_arr) {
  rapidjson::Document json_doc;
  json_doc.SetObject();
  parse_msg_rapidjson(msg, print_empty_arr, json_doc);
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  json_doc.Accept(writer);
  return buffer.GetString();
}

void parse_msg_rapidjson(const google::protobuf::Message *msg,
                         bool print_empty_arr,
                         rapidjson::Document& json_doc) {
  auto& json_alloc = json_doc.GetAllocator();
  const google::protobuf::Descriptor* d = msg->GetDescriptor();
  if (!d) {
    return;
  }

  size_t count = d->field_count();
  for (size_t i = 0; i != count ; ++i) {
    const google::protobuf::FieldDescriptor* field = d->field(i);
    if (!field) {
      continue;
    }
    const google::protobuf::Reflection* ref = msg->GetReflection();
    if (!ref) {
      continue;
    }
    rapidjson::Value json_key(rapidjson::kStringType);
    json_key.SetString(field->name().c_str(), field->name().length(), json_alloc);

    if (field->is_repeated()) {
      const int count = ref->FieldSize(*msg, field);
      if (count > 0 || print_empty_arr) {
        rapidjson::Value json_value_array(rapidjson::kArrayType);
        parse_repeated_field_rapidjson(msg, ref, field, print_empty_arr, json_value_array, json_doc);
        json_doc.AddMember(json_key, json_value_array, json_alloc);
      }
    } else if (ref->HasField(*msg, field)) {
      double value1;
      float value2;
      int64_t value3;
      uint64_t value4;
      int32_t value5;
      uint32_t value6;
      bool value7;
      std::string value8;
      const google::protobuf::Message *value9;
      const google::protobuf::EnumValueDescriptor *value10;
      rapidjson::Value json_value_number(rapidjson::kNumberType);
      rapidjson::Value json_value_string(rapidjson::kStringType);
      rapidjson::Value json_value_object(rapidjson::kObjectType);

      switch (field->cpp_type()) {
        case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
          value1 = ref->GetDouble(*msg, field);
          json_value_number.SetDouble(value1);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
          value2 = ref->GetFloat(*msg, field);
          json_value_number.SetDouble(value2);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
          value3 = ref->GetInt64(*msg, field);
          json_value_number.SetInt64(value3);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
          value4 = ref->GetUInt64(*msg, field);
          json_value_number.SetUint64(value4);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
          value5 = ref->GetInt32(*msg, field);
          json_value_number.SetInt(value5);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
          value6 = ref->GetUInt32(*msg, field);
          json_value_number.SetUint(value6);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
          value7 = ref->GetBool(*msg, field);
          json_value_number.SetBool(value7);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
          value8 = ref->GetString(*msg, field);
          json_value_string.SetString(value8.c_str(), value8.length(), json_alloc);
          json_doc.AddMember(json_key, json_value_string, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
          value9 = &(ref->GetMessage(*msg, field));
          parse_msg_rapidjson2(value9, print_empty_arr, json_value_object, json_doc);
          json_doc.AddMember(json_key, json_value_object, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
          value10 = ref->GetEnum(*msg, field);
          json_value_number.SetInt(value10->number());
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        default:
          break;
      }
    }
  }
}

void parse_repeated_field_rapidjson(const google::protobuf::Message *msg,
                                    const google::protobuf::Reflection *ref,
                                    const google::protobuf::FieldDescriptor *field,
                                    bool print_empty_arr,
                                    rapidjson::Value& json_value_array,
                                    rapidjson::Document& json_doc) {
  auto& json_alloc = json_doc.GetAllocator();
  const int count = ref->FieldSize(*msg, field);
  if (count <= 0) {
    return;
  }
  switch (field->cpp_type()) {
    case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
      for (int i = 0; i < count; i++) {
        double value1 = ref->GetRepeatedDouble(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetDouble(value1);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
      for (int i = 0; i < count; i++) {
        float value2 = ref->GetRepeatedFloat(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetDouble(value2);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
      for (int i = 0; i < count; i++) {
        int64_t value3 = ref->GetRepeatedInt64(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetInt64(value3);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
      for (int i = 0; i < count; i++) {
        uint64_t value4 = ref->GetRepeatedUInt64(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetUint64(value4);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
      for (int i = 0; i < count; i++) {
        int32_t value5 = ref->GetRepeatedInt32(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetInt(value5);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
      for (int i = 0; i < count; i++) {
        uint32_t value6 = ref->GetRepeatedUInt32(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetUint(value6);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
      for (int i = 0; i < count; i++) {
        bool value7 = ref->GetRepeatedBool(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetBool(value7);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
      for (int i = 0; i < count; i++) {
        std::string value8 = ref->GetRepeatedString(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kStringType);
        json_value.SetString(value8.c_str(), value8.length(), json_alloc);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
      for (int i = 0; i < count; i++) {
        auto* value9 = &(ref->GetRepeatedMessage(*msg, field, i));
        rapidjson::Value json_value(rapidjson::kObjectType);
        parse_msg_rapidjson2(value9, print_empty_arr, json_value, json_doc);
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
      for (int i = 0; i < count; i++) {
        auto* value10 = ref->GetRepeatedEnum(*msg, field, i);
        rapidjson::Value json_value(rapidjson::kNumberType);
        json_value.SetInt(value10->number());
        json_value_array.PushBack(json_value, json_alloc);
      }
      break;
    default:
      break;
  }
}

void parse_msg_rapidjson2(const google::protobuf::Message *msg,
                          bool print_empty_arr,
                          rapidjson::Value& json_doc,
                          rapidjson::Document& json_doc_ori) {
  auto& json_alloc = json_doc_ori.GetAllocator();
  const google::protobuf::Descriptor *d = msg->GetDescriptor();
  if (!d) {
    return;
  }

  size_t count = d->field_count();
  for (size_t i = 0; i != count ; ++i) {
    const google::protobuf::FieldDescriptor *field = d->field(i);
    if (!field) {
      continue;
    }
    const google::protobuf::Reflection *ref = msg->GetReflection();
    if (!ref) {
      continue;
    }
    rapidjson::Value json_key(rapidjson::kStringType);
    json_key.SetString(field->name().c_str(), field->name().length(), json_alloc);

    if (field->is_repeated()) {
      const int count = ref->FieldSize(*msg, field);
      if (count > 0 || print_empty_arr) {
        rapidjson::Value json_value_array(rapidjson::kArrayType);
        parse_repeated_field_rapidjson(msg, ref, field, print_empty_arr, json_value_array, json_doc_ori);
        json_doc.AddMember(json_key, json_value_array, json_alloc);
      }
    } else if (ref->HasField(*msg, field)) {
      double value1;
      float value2;
      int64_t value3;
      uint64_t value4;
      int32_t value5;
      uint32_t value6;
      bool value7;
      std::string value8;
      const google::protobuf::Message *value9;
      const google::protobuf::EnumValueDescriptor *value10;
      rapidjson::Value json_value_number(rapidjson::kNumberType);
      rapidjson::Value json_value_string(rapidjson::kStringType);
      rapidjson::Value json_value_object(rapidjson::kObjectType);

      switch (field->cpp_type()) {
        case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
          value1 = ref->GetDouble(*msg, field);
          json_value_number.SetDouble(value1);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
          value2 = ref->GetFloat(*msg, field);
          json_value_number.SetDouble(value2);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
          value3 = ref->GetInt64(*msg, field);
          json_value_number.SetInt64(value3);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
          value4 = ref->GetUInt64(*msg, field);
          json_value_number.SetUint64(value4);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
          value5 = ref->GetInt32(*msg, field);
          json_value_number.SetInt(value5);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
          value6 = ref->GetUInt32(*msg, field);
          json_value_number.SetUint(value6);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
          value7 = ref->GetBool(*msg, field);
          json_value_number.SetBool(value7);
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
          value8 = ref->GetString(*msg, field);
          json_value_string.SetString(value8.c_str(), value8.length(), json_alloc);
          json_doc.AddMember(json_key, json_value_string, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
          value9 = &(ref->GetMessage(*msg, field));
          parse_msg_rapidjson2(value9, print_empty_arr, json_value_object, json_doc_ori);
          json_doc.AddMember(json_key, json_value_object, json_alloc);
          break;
        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
          value10 = ref->GetEnum(*msg, field);
          json_value_number.SetInt(value10->number());
          json_doc.AddMember(json_key, json_value_number, json_alloc);
          break;
        default:
          break;
      }
    }
  }
}
*/
