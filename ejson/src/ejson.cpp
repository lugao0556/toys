#include <assert.h>
#include <stdlib.h>
#include "ejson.h"


namespace JsonParser {

struct Context {
    std::string& _json;

    Context(std::string json) : _json(json) {};
};


int parse_null_type(JsonValue& jv, Context& c) {
    if (c._json[0] != 'n' || c._json[1] != 'u' || c._json[2] != 'l' || c._json[3] != 'l') {
        return PARSE_INVALID;
    }
    c._json = c._json.substr(4);
    jv.type = E_NULL;
    return PARSE_OK;
}

int parse_false_type(JsonValue& jv, Context& c) {
    if (c._json[0] != 'f' || c._json[1] != 'a' || c._json[2] != 'l' || c._json[3] != 's' || c._json[4] != 'e') {
        return PARSE_INVALID;
    }
    c._json = c._json.substr(5);
    jv.type = E_FALSE;
    return PARSE_OK;
}

int parse_true_type(JsonValue& jv, Context& c) {
    if (c._json[0] != 't' || c._json[1] != 'r' || c._json[2] != 'u' || c._json[3] != 'e') {
        return PARSE_INVALID;
    }
    c._json = c._json.substr(4);
    jv.type = E_TRUE;
    return PARSE_OK;
}

int parse_number_value(JsonValue& jv, Context& c) {
    char* end;
    const char* json = c._json.c_str();
    jv.num_value = strtod(json, &end);
    if (json == end) {
        return PARSE_INVALID;
    }

    json = end;
    c._json = json;

    jv.type = E_NUMBER;
    return PARSE_OK;
}

int parse_helper(JsonValue& jv, Context& c) {
    switch (c._json[0])
    {
    case 'n':  return parse_null_type(jv, c);
    case 'f':  return parse_false_type(jv, c);
    case 't':  return parse_true_type(jv, c);
    default:   return parse_number_value(jv, c);  
    }
}

void handle_whitspace(Context& c) {
    size_t index = 0;
    while (c._json[index] == ' ' || c._json[index] == '\t' || c._json[index] == '\n' || c._json[index] == '\r') {
        index++;
    }

    c._json = c._json.substr(index);
}

int parse(JsonValue& jv, const std::string& json) {
    assert(json.size() != 0);
    Context c(json);
    handle_whitspace(c);
    return parse_helper(jv, c);
}

JsonType get_type(const JsonValue& jv) {
    return jv.type;
}

double get_num_value(const JsonValue& jv) {
    assert(jv.type == E_NUMBER);
    return jv.num_value;
}

}