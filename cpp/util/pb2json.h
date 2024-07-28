#include <string>

#include <google/protobuf/message.h>

std::string pb2json(const google::protobuf::Message *msg, bool print_empty_arr);
