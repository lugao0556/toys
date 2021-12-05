#ifndef EJSON_H_
#define EJSON_H_

#include <string>

namespace JsonParser {

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
    PARSE_INVALID,
    PARSE_SINGUAR,
};

struct JsonValue
{
    JsonType type;
    double num_value;

};

extern int      parse(JsonValue& jv, const std::string& json);

extern JsonType get_type(const JsonValue& jv);
extern double get_num_value(const JsonValue& jv);

}


#endif