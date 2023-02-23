#include <util/base.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

std::string JsonDocToString(const rapidjson::Document& doc) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);
  return buffer.GetString();
}

std::string BuildJson() {
  rapidjson::Document json_doc;
  json_doc.SetObject();
  auto& json_alloc = json_doc.GetAllocator();

  rapidjson::Value arr(rapidjson::kArrayType);
  for (int i = 100; i <= 103; i++) {
    rapidjson::Value ele(rapidjson::kObjectType);
    rapidjson::Value key(("a" + std::to_string(i)).data(), json_alloc);
    rapidjson::Value val(i);
    ele.AddMember(key, val, json_alloc);
    arr.PushBack(ele, json_alloc);
  }
  json_doc.AddMember("arr", arr, json_alloc);

  auto json_str = JsonDocToString(json_doc);
  print(json_str);
  print();

  return json_str;
}

void ParseJson(const std::string& json_str) {
  rapidjson::Document json_doc;
  json_doc.Parse(json_str.data());
  if (json_doc.HasParseError()) {
    return;
  }

  auto arr_iter = json_doc.FindMember("arr");
  if (arr_iter == json_doc.MemberEnd()) {
    return;
  }
  auto& arr = *arr_iter;
  if (arr.value.GetType() != rapidjson::kArrayType) {
    return;
  }
  for (auto& ele : arr.value.GetArray()) {
    for (auto& kv : ele.GetObject()) {
      if (kv.value.GetType() != rapidjson::kNumberType) {
        continue;
      }
      prints(kv.name.GetString(), "=", kv.value.GetInt());
    }
  }
}

int main() {
  auto s = BuildJson();
  ParseJson(s);
}
