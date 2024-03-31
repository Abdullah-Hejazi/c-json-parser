#include "json.h"
#include <stdio.h>

int main(int argc, char** argv) {
    Pair pair = parse_json("data/test11.json");

    print_pair(pair);

    return 0;
}