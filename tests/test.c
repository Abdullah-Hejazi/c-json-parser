#include "../json.h"
#include "unity/unity.h"
#include <stdlib.h>

void setUp(void) {
}

void tearDown(void) {
}

void test_ReadFileWorks(void) {
	char* string = read_file("tests/data/test0.txt");

	TEST_ASSERT_EQUAL_STRING("hello", string);
}

void test_GetTypeStringWorks(void) {
	char* string = get_type_string(STRING);
	char* number = get_type_string(NUMBER);
	char* object = get_type_string(OBJECT);
	char* array = get_type_string(ARRAY);
	char* boolean = get_type_string(BOOLEAN);
	char* none = get_type_string(NONE);
	char* unknown = get_type_string((Type)50);

	TEST_ASSERT_EQUAL_STRING("string", string);
	TEST_ASSERT_EQUAL_STRING("number", number);
	TEST_ASSERT_EQUAL_STRING("object", object);
	TEST_ASSERT_EQUAL_STRING("array", array);
	TEST_ASSERT_EQUAL_STRING("boolean", boolean);
	TEST_ASSERT_EQUAL_STRING("NULL", none);
	TEST_ASSERT_EQUAL_STRING("???", unknown);
}

void test_ParseNumber(void) {
	char* data = "123.456";
	int cursor = 0;
	double value = parse_number(data, &cursor);

	TEST_ASSERT_EQUAL_FLOAT(123.456, value);
}

void test_ParseString(void) {
	char* data = "\"hello\\\" world\"   ";
	int cursor = 0;
	char* value = parse_string(data, &cursor);

	TEST_ASSERT_EQUAL_STRING("hello\\\" world", value);
}

void test_GetKey(void) {
	char* data = "{\"key\": \"value\"}";
	int cursor = 1;
	char* value = get_key(data, &cursor);

	TEST_ASSERT_EQUAL_STRING("key", value);
}

void test_ParseObject(void) {
	char* data = "{\"key1\": \"value1\", \"key2\": 25.5, \"key3\": null}";
	int cursor = 0;

	Object* object = parse_object(data, &cursor);

	Pair* pair1 = object->pairs[0];
	Pair* pair2 = object->pairs[1];
	Pair* pair3 = object->pairs[2];

	TEST_ASSERT_EQUAL_STRING("key1", pair1->key);
	TEST_ASSERT_EQUAL_STRING("value1", pair1->value);

	TEST_ASSERT_EQUAL_STRING("key2", pair2->key);
	TEST_ASSERT_EQUAL_FLOAT(25.5, *(double*)pair2->value);

	TEST_ASSERT_EQUAL_STRING("key3", pair3->key);
	TEST_ASSERT_EQUAL(NULL, pair3->value);

	TEST_ASSERT_EQUAL(3, object->count);
}

void test_ParseArray(void) {
	char* data = "[\"value1\", 25.5, null]";
	int cursor = 0;

	Array* array = parse_array(data, &cursor);

	Pair* pair1 = array->pairs[0];
	Pair* pair2 = array->pairs[1];
	Pair* pair3 = array->pairs[2];

	TEST_ASSERT_EQUAL(NULL, pair1->key);
	TEST_ASSERT_EQUAL_STRING("value1", pair1->value);

	TEST_ASSERT_EQUAL(NULL, pair2->key);
	TEST_ASSERT_EQUAL_FLOAT(25.5, *(double*)pair2->value);

	TEST_ASSERT_EQUAL(NULL, pair2->key);
	TEST_ASSERT_EQUAL(NULL, pair3->value);

	TEST_ASSERT_EQUAL(3, array->count);
}

void test_ParseType(void) {
	char* data = "[\"value1\", 25.5, null]";
	int cursor = 0;
	Pair* pair = parse_type(data, NULL, &cursor);
	TEST_ASSERT_EQUAL(NULL, pair->key);
	TEST_ASSERT_EQUAL(ARRAY, pair->type);

	data = "{\"key1\": \"value1\", \"key2\": 25.5, \"key3\": null}";
	cursor = 0;
	pair = parse_type(data, NULL, &cursor);
	TEST_ASSERT_EQUAL(NULL, pair->key);
	TEST_ASSERT_EQUAL(OBJECT, pair->type);

	data = "25.5";
	cursor = 0;
	pair = parse_type(data, NULL, &cursor);
	TEST_ASSERT_EQUAL(NULL, pair->key);
	TEST_ASSERT_EQUAL(NUMBER, pair->type);

	data = "\"value1\"";
	cursor = 0;
	pair = parse_type(data, NULL, &cursor);
	TEST_ASSERT_EQUAL(NULL, pair->key);
	TEST_ASSERT_EQUAL(STRING, pair->type);

	data = "null";
	cursor = 0;
	pair = parse_type(data, NULL, &cursor);
	TEST_ASSERT_EQUAL(NULL, pair->key);
	TEST_ASSERT_EQUAL(NONE, pair->type);

	data = "true";
	cursor = 0;
	pair = parse_type(data, NULL, &cursor);
	TEST_ASSERT_EQUAL(NULL, pair->key);
	TEST_ASSERT_EQUAL(BOOLEAN, pair->type);

	data = "false";
	cursor = 0;
	pair = parse_type(data, NULL, &cursor);
	TEST_ASSERT_EQUAL(NULL, pair->key);
	TEST_ASSERT_EQUAL(BOOLEAN, pair->type);

	data = "unknown";
	cursor = 0;
	pair = parse_type(data, NULL, &cursor);
	TEST_ASSERT_EQUAL(NULL, pair->key);
	TEST_ASSERT_EQUAL(NONE, pair->type);
}

void test_ParseFullJson(void) {
	/*
	
	{
    "name": "User Name",
    "email": "test@test.com",
    "phone": "1234567890",
    "age": 25,
    "test": null,
    "hola": true,
    "new": {
        "user": "xx",
        "age": 11
    }
}
	*/

	Pair* pair = parse_json("tests/data/test1.json");

	TEST_ASSERT_EQUAL(7, ((Object*)(pair->value))->count);
	TEST_ASSERT_EQUAL(OBJECT, pair->type);
	
	Object* object = pair->value;
	Pair* pair1 = object->pairs[0];
	Pair* pair2 = object->pairs[1];
	Pair* pair3 = object->pairs[2];
	Pair* pair4 = object->pairs[3];
	Pair* pair5 = object->pairs[4];
	Pair* pair6 = object->pairs[5];
	Pair* pair7 = object->pairs[6];

	TEST_ASSERT_EQUAL_STRING("name", pair1->key);
	TEST_ASSERT_EQUAL(STRING, pair1->type);
	TEST_ASSERT_EQUAL_STRING("User Name", pair1->value);

	TEST_ASSERT_EQUAL_STRING("email", pair2->key);
	TEST_ASSERT_EQUAL(STRING, pair2->type);
	TEST_ASSERT_EQUAL_STRING("test@test.com", pair2->value);

	TEST_ASSERT_EQUAL_STRING("phone", pair3->key);
	TEST_ASSERT_EQUAL(STRING, pair3->type);
	TEST_ASSERT_EQUAL_STRING("1234567890", pair3->value);

	TEST_ASSERT_EQUAL_STRING("age", pair4->key);
	TEST_ASSERT_EQUAL(NUMBER, pair4->type);
	TEST_ASSERT_EQUAL_FLOAT(25, *(double*)pair4->value);

	TEST_ASSERT_EQUAL_STRING("test", pair5->key);
	TEST_ASSERT_EQUAL(NONE, pair5->type);
	TEST_ASSERT_EQUAL(NULL, pair5->value);

	TEST_ASSERT_EQUAL_STRING("hola", pair6->key);
	TEST_ASSERT_EQUAL(BOOLEAN, pair6->type);
	TEST_ASSERT_EQUAL(1, *(bool*)pair6->value);

	TEST_ASSERT_EQUAL_STRING("new", pair7->key);
	TEST_ASSERT_EQUAL(OBJECT, pair7->type);

	Object* new = pair7->value;
	Pair* newPair1 = new->pairs[0];
	Pair* newPair2 = new->pairs[1];

	TEST_ASSERT_EQUAL_STRING("user", newPair1->key);
	TEST_ASSERT_EQUAL(STRING, newPair1->type);
	TEST_ASSERT_EQUAL_STRING("xx", newPair1->value);

	TEST_ASSERT_EQUAL_STRING("age", newPair2->key);
	TEST_ASSERT_EQUAL(NUMBER, newPair2->type);
	TEST_ASSERT_EQUAL_FLOAT(11, *(double*)newPair2->value);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ReadFileWorks);
    RUN_TEST(test_GetTypeStringWorks);
    RUN_TEST(test_ParseNumber);
    RUN_TEST(test_ParseString);
    RUN_TEST(test_GetKey);
    RUN_TEST(test_ParseObject);
    RUN_TEST(test_ParseArray);
    RUN_TEST(test_ParseType);
    RUN_TEST(test_ParseFullJson);
    return UNITY_END();
}