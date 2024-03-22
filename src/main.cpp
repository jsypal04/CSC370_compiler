#include <iostream>
#include "headers/parser.h"
using namespace std;

int main() {

    Parser parser;

    parser.parse();

    parser.destroyLexer();

    return 0;
}