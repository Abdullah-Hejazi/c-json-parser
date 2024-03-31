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
    // double* number;
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
Pair parse_json(char*);
void print_pair(Pair pair);

#endif