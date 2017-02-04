#include "hw1.h"
#include "info.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;
    int mode;

    /* Note: create a variable to assign the result of validargs */
    mode = validargs(argc, argv, &in, &out);
    // printf("%s\n", "return mode value!");
    // printf("%d\n", mode);
    if(mode == -128){
    	USAGE(mode);
    	return EXIT_SUCCESS;
    }
    else if (64<=mode && mode<=95){
        // printf("%s\n", "Substitution Encode");
        encode(in, out, mode-64);
        info("test ok");
    }
    else if (96<=mode && mode<=127){
        // printf("%s\n", "Substitution Dncode");
        decode(in, out, mode-96);
        info("test ok");

    }
    else if (1<=mode && mode<=31){
        // printf("%s\n", "Tutnese Encode");
        encode_tutnese(in,out);
    }
    else if (32<=mode && mode<=63){
        // printf("%s\n", "Tutnese Dncode");
        decode_tutnese(in, out);
    }
    else{
        USAGE(mode);
    	return EXIT_FAILURE;
    }
}
