#ifndef JSON_LIBRARY_H
#define JSON_LIBRARY_H

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
    char* value;

    Type type;
} Pair;

#endif