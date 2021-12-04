#include<string>
#include <stdio.h>
#include <stdlib.h>
#include "ejson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

void test_parse_null() {
    JsonParser::JsonValue jv;
    jv.type = JsonParser::E_NULL;
    EXPECT_EQ_INT(JsonParser::PARSE_OK, JsonParser::parse(jv, "null"));
    EXPECT_EQ_INT(JsonParser::E_NULL, JsonParser::get_type(jv));
}

/* ... */

void test_parse() {
    test_parse_null();
    /* ... */
}

