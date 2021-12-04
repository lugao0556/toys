#ifndef EJSON_H_
#define EJSON_H_

#include <string>



enum JsonType {
    E_NULL,
    E_FALSE,
    E_TRUE,
    E_NUMBER,
    E_STRING,
    E_ARRAY,
    E_OBJECT,
};

enum ErrorCode {
    PARSE_OK,
    PARSE_EXPECT,
    PARSE_UNEXPECT,
    PARSE_SINGUAR,
};

struct JsonValue
{
    JsonType type;

    int parse(JsonValue& jv, const std::string& json);
    JsonType get_type(JsonValue& jv) {return jv.type;}
};


#endif