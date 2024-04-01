#include "json.h"
#include <stdio.h>

int main(int argc, char** argv) {
    Pair* pair = parse_json("data/complex1.json");

    print_pair(pair, 0);

    return 0;
}