#include "hw1.h"
#include "info.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;
    int mode;
    char* input = 0;
    char* output = 0;

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
        fclose(in);
        fclose(out);
        input = *(argv+3);
        output = *(argv+4);

        if(*(input)=='-')
            input = "STDIN";
        if(*(output)=='-')
            output = "STDOUT";
        if (argc==6)
            info(Alphabet,*(argv+5),input,output,"Encrption");
        else
            info(Alphabet,"320",input,output,"Encrption");

    }
    else if (96<=mode && mode<=127){
        // printf("%s\n", "Substitution Dncode");
        decode(in, out, mode-96);
        fclose(in);
        fclose(out);
        input = *(argv+3);
        output = *(argv+4);

        if(*(input)=='-')
            input = "STDIN";
        if(*(output)=='-')
            output = "STDOUT";
        if (argc==6)
            info(Alphabet,*(argv+5),input,output,"Decrption");
        else
            info(Alphabet,"320",input,output,"Decrption");

    }
    else if (1<=mode && mode<=31){
        // printf("%s\n", "Tutnese Encode");
        encode_tutnese(in,out);
        fclose(in);
        fclose(out);
    }
    else if (32<=mode && mode<=63){
        // printf("%s\n", "Tutnese Dncode");
        decode_tutnese(in, out);
        fclose(in);
        fclose(out);
    }
    else{
        USAGE(mode);
    	return EXIT_FAILURE;
    }

}
