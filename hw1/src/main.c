#include "hw1.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;

    /* Note: create a variable to assign the result of validargs */
    validargs(argc, argv, &in, &out);

    return EXIT_SUCCESS;
}
