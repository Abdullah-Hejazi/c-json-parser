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

void parse_json(char* filename) {
    char* data = read_file(filename);

    if (data[0] == '{') {
        printf("Object Found\n");
    } else if (data[0] == '[') {
        printf("Array Found\n");
    } else if (data[0] == '"') {
        printf("String Found\n");
    } else if (data[0] == 'n') {
        printf("Null Found\n");
    } else {
        printf("Number Found\n");
    }

    free(data);
}