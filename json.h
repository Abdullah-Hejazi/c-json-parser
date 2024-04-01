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


char* read_file(char* filename);
char* get_type_string(Type type);
void print_pair(Pair* pair, int padding);
double parse_number(char* data, int* cursor);
char* parse_string(char* data, int* cursor);
char* get_key(char* data, int* cursor);
Object* parse_object(char* data, int* cursor);
Array* parse_array(char* data, int* cursor);
Pair* parse_type(char* data, char* key, int* cursor);
Pair* parse_json(char* filename);

#endif