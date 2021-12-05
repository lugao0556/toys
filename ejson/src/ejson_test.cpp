#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
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

const double epsilon = 0.00001;
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((abs((expect) - (actual)) <= epsilon), expect, actual, "%lf")

#define TEST_NUMBER(expect, json) \
    do {\
        JsonParser::JsonValue jv;\
        EXPECT_EQ_INT(JsonParser::PARSE_OK, JsonParser::parse(jv, json));\
        EXPECT_EQ_INT(JsonParser::E_NUMBER, JsonParser::get_type(jv));\
        EXPECT_EQ_DOUBLE(expect, JsonParser::get_num_value(jv));\
    } while(0)

#define TEST_ERROR(error, json)\
    do {\
        JsonParser::JsonValue jv;\
        EXPECT_EQ_INT(error, JsonParser::parse(jv, json));\
    } while(0)

void test_parse_null() {
    JsonParser::JsonValue jv;
    EXPECT_EQ_INT(JsonParser::PARSE_OK, JsonParser::parse(jv, "      null"));
    EXPECT_EQ_INT(JsonParser::E_NULL, JsonParser::get_type(jv));
}

void test_parse_false() {
    JsonParser::JsonValue jv;
    EXPECT_EQ_INT(JsonParser::PARSE_OK, JsonParser::parse(jv, "      false"));
    EXPECT_EQ_INT(JsonParser::E_FALSE, JsonParser::get_type(jv));
}

void test_parse_true() {
    JsonParser::JsonValue jv;
    EXPECT_EQ_INT(JsonParser::PARSE_OK, JsonParser::parse(jv, "      true"));
    EXPECT_EQ_INT(JsonParser::E_TRUE, JsonParser::get_type(jv));
}

void test_parse_invalid_value() {
    /* ... */
    /* invalid number */
    TEST_ERROR(JsonParser::PARSE_INVALID, "+0");
    TEST_ERROR(JsonParser::PARSE_INVALID, "+1");
    TEST_ERROR(JsonParser::PARSE_INVALID, ".123"); /* at least one digit before '.' */
    TEST_ERROR(JsonParser::PARSE_INVALID, "1.");   /* at least one digit after '.' */
    TEST_ERROR(JsonParser::PARSE_INVALID, "INF");
    TEST_ERROR(JsonParser::PARSE_INVALID, "inf");
    TEST_ERROR(JsonParser::PARSE_INVALID, "NAN");
    TEST_ERROR(JsonParser::PARSE_INVALID, "nan");
}

void test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */
}

/* ... */

void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_invalid_value();
    
    /* ... */
}

