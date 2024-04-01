#include "json.h"
#include <stdio.h>

int main(int argc, char** argv) {
    Pair* pair = parse_json("data/test14.json");

    print_pair(pair, 0);

    return 0;
}