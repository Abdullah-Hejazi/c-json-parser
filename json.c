#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(char* filename) {
    FILE* file = fopen(filename, "r");

    fseek(file, 0L, SEEK_END);

    int size = ftell(file);

    fseek(file, 0L, SEEK_SET);

    char* data = malloc(size + 1);

    fread(data, size, 1, file);

    data[size] = '\0';

    return data;
}

char* get_type_string(Type type) {
    switch(type) {
        case NUMBER: return "number";
        case STRING: return "string";
        case ARRAY: return "array";
        case OBJECT: return "object";
        case BOOLEAN: return "boolean";
        case NONE: return "NULL";
        default: return "???";
    }
}

void print_padding(int padding) {
    for(int i = 0; i < padding; i++) {
        printf("\t");
    }
}

void print_pair(Pair* pair, int padding) {
    if (pair == NULL) {
        return;
    }

    if (pair->key != NULL) {
        print_padding(padding);

        printf("%s = ", pair->key);
    }

    if (pair->type == NONE) {
        printf("null");
    }

    else if (pair->type == STRING) {
        char* string = (char*)pair->value;
        printf("\"%s\"", string);
    }

    else if (pair->type == NUMBER) {
        double number = *((double*)pair->value);
        printf("%f", number);
    }

    else if (pair->type == OBJECT) {
        printf("OBJECT {\n");
        padding++;

        for(int i = 0; i < (*((Object*)pair->value)).count; i++) {
            printf("\t");
            print_pair((*((Object*)pair->value)).pairs[i], padding);
        }

        print_padding(padding);
        printf("}(%d)", (*((Object*)pair->value)).count);
    }

    else if (pair->type == ARRAY) {
        printf("ARRAY [\n");
        padding++;

        for(int i = 0; i < (*((Array*)pair->value)).count; i++) {
            printf("\t");
            print_pair((*((Array*)pair->value)).pairs[i], padding);
        }

        print_padding(padding);
        printf("](%d)", (*((Array*)pair->value)).count);
    }

    else if (pair->type == BOOLEAN) {
        if (*((bool*)pair->value) == 0) {
            printf("false");
        } else {
            printf("true");
        }
    }


    // printf("\t// %s\n", get_type_string(pair->type));
    printf("\n");
}

double parse_number(char* data, int* cursor) {
    double result = 0;
    bool dot = 0;
    double dotCounter = 1;

    while((data[*cursor] >= 48 && data[*cursor] <= 57) || data[*cursor] == 46) {
        if (data[*cursor] == 46) {
            dot = 1;
            *cursor += 1;
            continue;
        }

        if (dot == 0) {
            result *= 10;
            result += data[*cursor] - 48;
        } else {
            result += (double)(data[*cursor] - 48) / (10 * dotCounter);
            dotCounter *= 10;
        }

        *cursor += 1;
    }

    return result;
}

char* parse_string(char* data, int* cursor) {
    *cursor += 1;

    int string_start = *cursor;
    int string_end = *cursor;

    while (*cursor < strlen(data)) {
        if (data[*cursor] == '"' && data[*cursor - 1] != '\\') {
            string_end = *cursor;
            break;
        }

        *cursor += 1;
    }

    if (string_start == string_end) {
        char* empty = malloc(1);
        *empty = '\0';
        return empty;
    }

    char* result = malloc(string_end - string_start + 1);

    memcpy(result, &data[string_start], string_end - string_start);

    result[string_end - string_start] = '\0';

    *cursor += 1;
    return result;
}

char* get_key(char* data, int* cursor) {
    while(*cursor < strlen(data)) {
        if (data[*cursor] == '"') {
            return parse_string(data, cursor);
        }

        if (data[*cursor] == '}') {
            return NULL;
        }

        *cursor += 1;
    }

    return NULL;
}

Object* parse_object(char* data, int* cursor) {
    *cursor += 1;

    Object* object = malloc(sizeof(Object));
    object->count = 0;

    while (*cursor < strlen(data)) {
        char* key = get_key(data, cursor);

        if (key == NULL) {
            break;
        }

        Pair* pair = parse_type(data, key, cursor);

        if (pair == NULL) {
            printf("Found key: [%s] but without a value !!", key);
            exit(1);
        }

        object->count += 1;

        if (object->count == 1) {
            object->pairs = malloc(sizeof(Pair));
        } else {
            object->pairs = realloc(object->pairs, object->count * sizeof(Pair));
        }

        object->pairs[object->count - 1] = pair;
    }

    *cursor += 1;

    return object;
}

Array* parse_array(char* data, int* cursor) {
    *cursor += 1;

    Array* array = malloc(sizeof(Array));
    array->count = 0;

    while (*cursor < strlen(data)) {
        Pair* pair = parse_type(data, NULL, cursor);

        if (pair == NULL) {
            *cursor += 1;
            break;
        }

        if (array->count == 0) {
            array->pairs = malloc(sizeof(Pair));
        } else {
            array->pairs = realloc(array->pairs, (array->count + 1) * sizeof(Pair));
        }

        array->pairs[array->count] = pair;
        array->count += 1;

        if (data[*cursor] == ']') {
            *cursor += 1;
            break;
        }

        *cursor += 1;
    }

    return array;
}

Pair* parse_type(char* data, char* key, int* cursor) {
    Pair* result = malloc(sizeof(Pair));
    result->key = key;

    while(*cursor < strlen(data)) {
        if (data[*cursor] == '{') {
            result->value = parse_object(data, cursor);
            result->type = OBJECT;
            break;
        } else if (data[*cursor] == '[') {
            result->value = parse_array(data, cursor);
            result->type = ARRAY;
            break;
        } else if (data[*cursor] == '"') {
            result->value = parse_string(data, cursor);
            result->type = STRING;
            break;
        } else if (data[*cursor] == 'n') {
            *cursor += 4;

            result->value = NULL;
            result->type = NONE;
            break;
        } else if (data[*cursor] == 't') {
            *cursor += 4;
            bool* value = malloc(sizeof(bool));
            *value = 1;
            
            result->value = value;
            result->type = BOOLEAN;
            break;
        } else if (data[*cursor] == 'f') {
            *cursor += 5;
            bool* value = malloc(sizeof(bool));
            *value = 0;
            
            result->value = value;
            result->type = BOOLEAN;
            break;
        } else if (data[*cursor] >= 48 && data[*cursor] <= 57) {
            double* value = malloc(sizeof(double));
            *value = parse_number(data, cursor);
            
            result->value = value;
            result->type = NUMBER;
            break;
        } else if (data[*cursor] == '}' || data[*cursor] == ']' || data[*cursor] == ',') {
            return NULL;
        }

        *cursor += 1;
    }

    return result;
}

Pair* parse_json(char* filename) {
    char* data = read_file(filename);

    int cursor = 0;

    return parse_type(data, NULL, &cursor);
}