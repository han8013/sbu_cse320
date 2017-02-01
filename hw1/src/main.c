#include "hw1.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;
    int flag;
    /* Note: create a variable to assign the result of validargs */
    flag = validargs(argc, argv, &in, &out);
    printf("%s\n", "return flag value!");
    printf("%d\n", flag);
    if(flag == -128){
    	USAGE(flag);
    	return EXIT_SUCCESS;
    }
    else if (64<=flag && flag<=95){
        printf("%s\n", "Substitution Encode");
    }
    else if (96<=flag && flag<=127){
        printf("%s\n", "Substitution Dncode");

    }
    else if (32<=flag && flag<=63){
        printf("%s\n", "Tutnese Dncode");
    }
    else if (1<=flag && flag<=31){
        printf("%s\n", "Tutnese Encode");
    }
    else{
    	printf("EXIT_FAILURE\n");
    	return EXIT_FAILURE;
    }
}
