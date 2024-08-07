#include <util/base.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

std::string JsonDocToString(const rapidjson::Document& doc, bool pretty = true) {
  rapidjson::StringBuffer buffer;
  if (pretty == false) {
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
  } else {
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
  }
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
  if (not arr.value.IsArray()) {
    return;
  }

  for (auto& ele : arr.value.GetArray()) {
    for (auto& kv : ele.GetObject()) {
      if (not kv.value.IsInt()) {
        continue;
      }
      prints(kv.name.GetString(), "=", kv.value.GetInt());
    }
  }
  print();

  for (auto ele = arr.value.Begin(); ele != arr.value.End(); ++ele) {
    if (not ele->IsObject()) {
      continue;
    }
    for (auto kv = ele->MemberBegin(); kv != ele->MemberEnd(); ++kv) {
      if (not kv->value.IsInt()) continue;
      prints(kv->name.GetString(), "=", kv->value.GetInt());
      kv->value.SetInt(300);
    }
    {
      rapidjson::StringBuffer val_buffer;
      rapidjson::Writer<rapidjson::StringBuffer> val_writer(val_buffer);
      ele->Accept(val_writer);
      println("raw json: ", val_buffer.GetString());
    }
  }
}

int main() {
  auto s = BuildJson();
  ParseJson(s);
}
