#include "hw1.h"

// For your helper functions (you may add additional files also)
// DO NOT define a main function here!

char validargs(int argc, char** argv, FILE** in, FILE** out) {
	char ret = 0;
	/*point to seconde argument, which is -h/-s/-t, first argument is bin/hw1*/
	char* dash1 = *(argv+1);
	/*first position is '-', seconde is h or s or t*/
	// printf("%d\n", *(dash1+1));
    if ('h'== *(dash1+1)){
    	ret = ret | -128;
    	ret = ret & -128;
        // printf("%s%d\n", "number of argument",argc);
    	// printf("%s\n", "return help mode");
    	// printf("%d\n", ret);
    }
    else if('s'== *(dash1+1) && argc>=5){
    	// printf("%s\n", "return part2 mode");
    	// printf("%d\n", *dash1);
    	/*point to third position, which -e/-d, fourth and fifth are infile and outfile.*/
    	char* dash2 = *(argv+2);
    	if ('e'== *(dash2+1)){
    	    // printf("%d\n", *dash2);
    		ret = 64;
            if (argc == 6){
                /*point to sixth position, which is n*/
                ret = ret + getFileNumber(*(argv+5)) % (getLength(Alphabet));
            }
            else{
                ret = ret + 320 % (getLength(Alphabet));
            }
    	}
    	else if ('d'== *(dash2+1)){
    	    // printf("%d\n", *(dash2+1));
    		ret = 96;
            if (argc == 6){
                /*point to sixth position, which is n*/
                ret = ret + getFileNumber(*(argv+5)) % (getLength(Alphabet));
            }
            else{
                ret = ret + 320 % (getLength(Alphabet));
            }
    	}
        // else{
        //     ret = 0;
        // }
    }
    else if('t'== *(dash1+1) && argc>=5){
    	// printf("%s\n", "return part3 mode");
    	// printf("%d\n", *(dash1+1));point to third position, which -e/-d, fourth and fifth are infile and outfile.
    	char* dash2 = *(argv+2);
    	if ('e'== *(dash2+1)){
    	    // printf("%d\n", *(dash2+1));
            // printf("%d\n", ret);
            // printf("%d\n", getLength(Alphabet));

    		ret = ret + 320 % (getLength(Alphabet)); // any non-zero number
    	   // printf("%d\n", ret);

        }
    	else if ('d'== *(dash2+1)){
    	    // printf("%d\n", *(dash2+1));
    		ret = 32;
            ret = ret + 320 % (getLength(Alphabet)); // any non-zero number
    	}
        // else{
        //     ret = 0;
        // }
    }
    // else{
    // 	printf("%s\n", "failure !!!");
    // }

    /*file open*/
    if (argc>=5){
        if (**(argv+3) =='-'){
            *in = stdin;
        }
        else{
            *in = fopen(*(argv+3), "r");
        }
        if (**(argv+4) =='-'){
            *out = stdout;
        }
        else{
            *out = fopen(*(argv+4), "w");
        }
    }
	return ret;
}

int getLength(char *start){
    int length = 0;
    int offset = 0;
    while (*(start + offset) != '\0'){
        length++;
        offset++;
    }
    return length;
}

int getFileNumber(char *number){
    int count = getLength(number);
	int result = 0;
    for (int i = 0; i < count; i++){
        // printf("%d\n", (*number)-48);
        result = result*10 + (*number)-48;
        number++;
    }
    // printf("%s%d\n", "n is ", result);
	return result;
}




