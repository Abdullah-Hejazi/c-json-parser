#ifndef JSON_LIBRARY_H
#define JSON_LIBRARY_H

typedef char bool;

typedef enum {
    STRING,
    NUMBER,
    OBJECT,
    ARRAY,
    BOOLEAN,
    NONE // NULL
} Type;

typedef struct {
    char* key;

    // char* string;
    // float* number;
    // bool* boolean;
    // Object* object;

    void* value;

    Type type;
} Pair;

typedef struct {
    Pair* pairs;

    bool isArray;
} Object;


char* read_file(char*);
void parse_json(char*);

#endif