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
    void* value;

    Type type;
} Pair;

typedef struct {
    Pair** pairs;
    int count;
} Object;

typedef struct {
    Pair** pairs;
    int count;
} Array;


char* read_file(char*);
Pair* parse_json(char*);
void print_pair(Pair* pair, int padding);
Pair* parse_type(char* data, char* key, int* cursor);

#endif