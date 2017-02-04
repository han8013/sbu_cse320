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
    }


    /*file open*/
    if (argc>=5){
        if (**(argv+3) =='-'){
            *in = stdin;
        }
        else{
            *in = fopen(*(argv+3), "r");
            if (in == NULL)
                return 0;
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

int getLengthOfArray(char **start){
    int length = 0;
    int offset = 0;
    while (*(start + offset) != NULL){
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

char toUpperCase(char lowCase){
    if (lowCase>=97 && lowCase<=122){
        lowCase = lowCase - 32;
    }
    return lowCase;
}

void encode(FILE* in, FILE *out, int n){
    int length = 0;
    int offset = 0;
    char c;
    while((c = getc(in) )!= EOF){
        c = replaceByEncode(toUpperCase(c),n);
        printf("%c", c);
        /* write to outfile*/
        if (out!=stdout){
            fprintf(out, "%c", c);
        }
        length++;
        offset++;
    }
}

void decode(FILE* in, FILE *out, int n){
    int length = 0;
    int offset = 0;
    char c;
    while((c = getc(in) )!= EOF){
        c = replaceByDecode(toUpperCase(c),n);
        printf("%c", c);
        /* write to outfile*/
        if (out!=stdout){
            fprintf(out, "%c", c);
        }
        length++;
        offset++;
    }
}


char replaceByEncode(char original, int n){
    int flag = 0;
    char replaced;
    int size = getLength(Alphabet);
    for (int i = 0; i < size; i++){
        if(original == *(Alphabet+i)){
            // printf("%c", original);
            replaced = *(Alphabet+((i+n)%size));
            flag = 1;
        }
    }
    if (flag == 1){
        return replaced;
    }
    else{
        return original;
    }
}

char replaceByDecode(char original, int n){
    int flag = 0;
    char replaced;
    int size = getLength(Alphabet);
    for (int i = 0; i < size; i++){
        if(original == *(Alphabet+i)){
            // printf("%c", original);
            replaced = *(Alphabet+((i+(size-n))%size));
            flag = 1;
        }
    }
    if (flag == 1){
        return replaced;
    }
    else{
        return original;
    }
}
int fileCharacterNumber(FILE *in){
    fseek(in, 0, SEEK_END);
    int length = ftell(in);
    return length;
}

int isUpper(char c){
    if (c-'A'>=32)
    {
        return 0;
    }
    return 1;
}

void decode_tutnese(FILE* in, FILE *out){
    int length = fileCharacterNumber(in);
    printf("%d\n", length);
    fseek(in,0,SEEK_SET);
    char curChar;
    char cq;
    char ct;
    char cvowel;
    int offset;
    // start position
    curChar = fgetc(in);
    for (int i = 1; i <= length; i++)
    {
        if (('A'<=curChar&&curChar<='Z') || ('a'<=curChar&&curChar<='z'))
        {
            if (isVowel(curChar))
            {
                fprintf(out, "%c", curChar);

                curChar = getc(in);
            }
            else
            {
                if (curChar == 's' || curChar == 'S')
                {

                    if ((cq = fgetc(in))== 'q')
                    {
                        i++;
                        // printf("%s\n", "double show!!!\n");
                        fseek(in,2,SEEK_CUR);
                        i=i+2;
                        if ((ct = fgetc(in))== 't' || ct == 'T')
                        {
                            i++;
                            /* do t+vowel, two characters check */
                            cvowel = fgetc(in);
                            offset = checkTForVowel(out,cvowel,curChar,ct);
                            i++;
                            fseek(in,offset,SEEK_CUR);
                            i = i+offset;
                            curChar = fgetc(in);
                        }
                    }
                    else{
                        /* do consonant check */
                        offset = foundInArray(in,out,curChar,curChar);
                        fseek(in,offset-1,SEEK_CUR); // offset-1 because if(cq == 'q' use fgetc once )
                        i = i+offset;
                        curChar = fgetc(in);
                    }
                }
                else{
                    /* do consonant check */

                    offset = foundInArray(in,out,curChar,curChar);
                    fseek(in,offset,SEEK_CUR);
                    i = i+offset;
                    curChar = fgetc(in);
                }
            }
        }
        else{
            fprintf(out, "%c", curChar);
            curChar = fgetc(in);
        }
    }
}

int foundInArray(FILE* in, FILE* out, char c,char isUpper){
    // int offset = 0;
    // offset++;
    // *buffer = c;

    int found = 0;
    char *fString;
    char fChar;
    int length = getLengthOfArray(Tutnese);
    for (int i = 0; i < length; ++i)
    {
        fString = *(Tutnese+i);
        fChar = *fString;
        if (c == fChar || (c+32) == fChar)
        {
            // printf("%s%c\n","Found CHAR is--",currentChar );
            // if ((isUpper-'A')>=32)
            // {
            //     fprintf(out, "%c%c", c,c);
            // }else{
            //     fprintf(out, "%c%c", c-32,c);
            // }
            // fprintf(out, "%c", c);
            found = getLength(fString)-1;
        }
    }
    fprintf(out, "%c", c);
    return found;
}

int checkTForVowel(FILE* out, char c, char isUpper, char t){

    int flag = 0;
    int found = 0;
    char *fString;
    char fChar;
    int length = getLengthOfArray(Tutnese);
    for (int i = 0; i < length; ++i)
    {
        fString = *(Tutnese+i);
        fChar = *fString;
        if (fChar == 't')
        {
            flag = 1;
            fChar = *(fString+1);
            if (c == fChar)
            {
                if ((isUpper-'A')>=32)
                {
                    fprintf(out, "%c%c", 't',t);
                }else{
                    fprintf(out, "%c%c", 'T',t);
                }
                found = getLength(fString)-2; // "tut" , pointer is 'u'
            }
            else{
                if ((isUpper-'A')>=32)
                {
                    fprintf(out, "%c%c", c,c);
                }else{
                    fprintf(out, "%c%c", c-32,c);
                }

            }
        }
    }
    if (flag == 0)
    {
        if ((isUpper-'A')>=32)
        {
            fprintf(out, "%c%c", c,c);
        }else{
            fprintf(out, "%c%c", c-32,c);
        }
    }

    return found;
}





void encode_tutnese(FILE* in, FILE *out){
    int length = fileCharacterNumber(in);
    // printf("%d\n", length);
    fseek(in,0,SEEK_SET);
    char c;
    char c2;
    if (length<=1)
    {
        for (int i = 1; i <= length; i++)
        {
            c = fgetc(in);
            encryption(out,c);
        }
    }
    else
    {
        c = fgetc(in);
        // printf("%s%c\n", "First Character--",c);
        for (int i = 1; i <= length; i++)
        {
            if (i<length)
            {
                c2 = fgetc(in); /*got secode char*/
                // printf("%s%c\n", "Next Character--",c2);
                if ((isDouble(c,c2))&&(((c>='a')&&(c<='z'))||((c>='A')&&(c<'Z'))))
                {
                    i++;
                    if (isUpper(c))
                        fprintf(out, "%s", "Squa");
                    else
                        fprintf(out, "%s", "squa");
                    if (isVowel(c))
                    {
                        fprintf(out, "%c", 't');
                    }
                    encryption(out,c2);
                    // printf("%d%s\n", i,"?");
                    // printf("%s%c\n", "Third Character--",c);
                    c = fgetc(in);
                }
                else
                {
                    encryption(out,c);
                    // printf("%d%s\n", i,"?");
                    c = c2;
                    // printf("%s%c\n", "C-2 assign to C--",c);
                }
            }
            else{
                encryption(out,c);
                // printf("%d%s\n", i,"?");
            }
        }
    }
}

int isDouble(char c, char c2){
    if (c==c2 || c == c2-32 || c == c2+32)
    {
        return 1;
    }
    return 0;
}

int isVowel(char c){
    if (c == 'a'|| c == 'e'|| c == 'i' || c == 'o'|| c =='u'||(c+32) == 'a'|| (c+32) == 'e'|| (c+32) == 'i' || (c+32) == 'o'|| (c+32) =='u')
    {
        return 1;
    }
    return 0;
}

void encryption(FILE *out,char c){
    int found = 0;
    char *currentString;
    char currentChar;
    int length = getLengthOfArray(Tutnese);
    for (int i = 0; i < length; ++i)
    {
        currentString = *(Tutnese+i);
        currentChar = *currentString;
        if (c == currentChar || (c+32)==currentChar)
        {
            // printf("%s%c\n","Found CHAR is--",currentChar );
            printString(out, currentString, isUpper(c));
            found = 1;
            // printf("%s%s\n","Found string is--",result );
        }
    }
    if (found == 0)
    {
        fprintf(out, "%c", c);
    }
}

void printString(FILE* out, char* s, int isCapital){
    int offset = 1;
    int length = getLength(s);
    if (isCapital)
    {
        fprintf(out, "%c", *s-32);

    }
    else{
        fprintf(out, "%c", *s);
    }
    while(length>offset){
        fprintf(out, "%c", *(s+offset));
        offset++;
    }
}







