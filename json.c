#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(char* filename) {
    FILE* file = fopen(filename, "r");

    fseek(file, 0L, SEEK_END);

    int size = ftell(file);

    fseek(file, 0L, SEEK_SET);

    char* data = malloc(size);

    fread(data, size, 1, file);

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

void print_pair(Pair pair) {
    if (pair.key != NULL) {
        printf("%s = ", pair.key);
    }

    if (pair.type == NONE) {
        printf("null");
    }

    else if (pair.type == STRING) {
        char* string = (char*)pair.value;
        printf("\"%s\"", string);
    }

    else if (pair.type == NUMBER) {
        double number = *((double*)pair.value);
        printf("%f", number);
    }

    else if (pair.type == OBJECT) {
        printf("OBJECT");
    }

    else if (pair.type == ARRAY) {
        printf("ARRAY");
    }

    else if (pair.type == BOOLEAN) {
        if (*((bool*)pair.value) == 0) {
            printf("false");
        } else {
            printf("true");
        }
    }


    printf(" of type (%s)\n", get_type_string(pair.type));
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
            printf("%f\n", (double)(data[*cursor] - 48) / (10 * dotCounter));
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

    char* result = malloc(string_end - string_start);

    memcpy(result, &data[string_start], string_end - string_start);

    *cursor += 1;
    return result;
}

Pair parse_type(char* data, int* cursor) {
    if (data[*cursor] == '"') {
        char* str = parse_string(data, cursor);

        Pair result = {NULL, str, STRING};
        return result;
    } else if (data[*cursor] == 'n') {
        *cursor += 4;
        Pair pair = {NULL, NULL, NONE};
        return pair;
    } else if (data[*cursor] == 't') {
        *cursor += 4;
        bool value = 1;
        Pair pair = {NULL, &value, BOOLEAN};
        return pair;
    } else if (data[*cursor] == 'f') {
        *cursor += 5;
        bool value = 0;
        Pair pair = {NULL, &value, BOOLEAN};
        return pair;
    } else if (data[*cursor] >= 48 && data[*cursor] <= 57) {
        double value = parse_number(data, cursor);

        Pair result = {NULL, &value, NUMBER};
        return result;
    }
}

Pair parse_json(char* filename) {
    char* data = read_file(filename);

    int cursor = 0;

    if (data[cursor] == '{') {
        printf("Object Found\n");
    } else if (data[cursor] == '[') {
        printf("Array Found\n");
    }else {
        printf("ERROR PARSING\n");
        exit(0);
    }

    free(data);
    
    Pair result = {NULL, NULL, NONE};
    return result;
}