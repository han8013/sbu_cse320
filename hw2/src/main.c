#include "hw2.h"

int main(int argc, char *argv[]){
    char DEFAULT_DICT_FILE[]= "rsrc/dictionary.txt";
    FILE* DEFAULT_INPUT = stdin;
    FILE* DEFAULT_OUTPUT = stdout;
    //create dictionary
    if((dict = (struct dictionary*) malloc(sizeof(struct dictionary))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }

    if((m_list = (struct misspelled_word*) malloc(sizeof(struct misspelled_word*))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }
    m_list = NULL;

    struct Args args;
    // Set struct default values
    args.d = false;
    args.i = false;
    args.o = false;
    strcpy(args.dictFile, DEFAULT_DICT_FILE);
    aFlag = 0;
    dFlag = 0;


    // Make a loop index
    //int i;
    char line[MAX_SIZE];
    //Declare Files
    FILE* dFile;
    FILE* iFile = DEFAULT_INPUT;
    FILE* oFile = DEFAULT_OUTPUT;
    //Declare number of misspelled words
    nMis = 0;

    /*char opt = '\0'; XINGHAN
    for(i = 1; i< argc; i++)
    {
        char* currArg = argv[i];
        //there's a flag
        if(opt != '\0')
        {
            if(opt == 'd')
            {
                strcpy(args.dictFile, currArg);
                args.d = true;
            }
            if(opt == 'i')
            {
                strcpy(args.input, currArg);
                args.i = true;
                iFile = fopen(currArg, "r");
            }
            if(opt == 'o')
            {
                strcpy(args.output, currArg);
                args.o = true;
                oFile = fopen(currArg, "w");
            }
            opt = '\0';
        }
        else
        {
            if(strcmp(currArg, "-d") == 0)
                opt = 'd';
            if(strcmp(currArg, "-i") == 0)
                opt = 'i';
            if(strcmp(currArg, "-o") == 0)
                opt = 'o';
        }
    }*/

    int helpFlag = 0; //Keep track if there's h flag
    int nFlag = 0; //Keep track if anything wrong with An
    int opt = 0;
    while((opt = getopt(argc, argv, "ho:i:d:A:")) != -1) {
        switch(opt) {
            case 'h':
                USAGE();
                helpFlag = 1;
                break;
            case 'o':
                strcpy(args.output, optarg);
                args.o = true;
                oFile = fopen(optarg, "w");
                break;
            case 'i':
                strcpy(args.input, optarg);
                args.i = true;
                iFile = fopen(optarg, "r");
                break;
            case 'd':
                strcpy(args.dictFile, optarg);
                args.d = true;
                break;
            case 'A':
                aFlag = 1;
                nMis = atoi(optarg);
                if(nMis < 0 || nMis > 5)
                    nFlag = 1;
                break;
        }
    }

    // handle -h exit
    if(helpFlag == 1) {
        if(iFile != NULL) fclose(iFile);
        if(oFile != NULL) fclose(oFile);
        return EXIT_SUCCESS;
    }

    // handle incorrect -An input
    if(nFlag == 1) {
        USAGE();
        if(iFile != NULL) fclose(iFile);
        if(oFile != NULL) fclose(oFile);
        return EXIT_FAILURE;
    }

    dFile = fopen(args.dictFile, "r");

    if(iFile == NULL && args.i == true)
    {
        //printf("Unable to open: %s.\n", args.input);
        USAGE();
        if(iFile != NULL) fclose(iFile);
        if(oFile != NULL) fclose(oFile);
        return EXIT_FAILURE;
    }
    if(dFile == NULL)
    {
        //printf("Unable to open: %s.\n", args.dictFile); XINGHAN
        USAGE();
        if(iFile != NULL) fclose(iFile);
        if(oFile != NULL) fclose(oFile);
        return EXIT_FAILURE;
    }
    else
    {
        processDictionary(dFile);

    }

    strcpy(line,"\n--------INPUT FILE WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);

    while(!feof(iFile))
    {
        char word[MAX_SIZE];
        char* wdPtr = word;
        //char line[MAX_SIZE];
        char* character = line;

        memset(line, 0, MAX_SIZE + 1);
        fgets(line, MAX_SIZE+1, iFile);

        //if there isn't a space or newline at the end of the line, put one there
        if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
            strcat(line, " ");

        // change to be lower case
        int i = 0;
        while( line[i] ) {
            line[i] = (tolower(line[i]));
            //putchar(line[i]);
            i++;
        }

        //replaces spaces within a line with new lines
        while(*character != '\0') // XINGHAN
        {
            if(*character == ' ' || *character == '\n')
             {
                // delete punct
                char* punctb = wdPtr-1;
                //printf("char:%c",*punct);
                while(!((*punctb>='a' && *punctb<='z') || (*punctb>='A' && *punctb<='Z')) && punctb >= word)
                {
                    punctb--;
                }
                punctb++;
                char temp = *punctb;
                *punctb = '\0';
                char* punctf = word;
                //printf("char:%c",*punct);
                while(!((*punctf>='a' && *punctf<='z') || (*punctf>='A' && *punctf<='Z')) && punctf <= wdPtr)
                {
                    punctf++;
                }
                //int size = strlen(wdPtr)-strlen(punct);
                //printf("%d", size); //strlen(wdPtr)-strlen(punct)); XINGHAN

                wdPtr = NULL;  // XINGHAN
                wdPtr = punctf; //XINGHAN
                if(punctb - punctf > 0){
                    processWord(wdPtr);
                }

                *punctb = temp;
                wdPtr = word;
                strcat(wdPtr, " ");
                fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);

                memset(word, 0, MAX_SIZE + 1);
            }
            else
            {
                *(wdPtr++) = *character;
            }
            character++;
        }

        strcpy(wdPtr, "\n");
        fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);

        if(iFile == stdin)
            break;
    }

    // Create new dictionary if necessary
    if(dFlag) {
        // First get the name of the new dictionary
        char newDictName[MAX_SIZE] = "";
        char* dF = args.dictFile;
        char* dB = dF + strlen(args.dictFile) - 1;
        while(dB >= dF) {
            if(*dB == '/') {
                break;
            }
            dB --;
        }
        dB++;
        if(dB > dF) {
            dB--;
            *dB = '\0';
            strcpy(newDictName, dF);
            dB++;
            strcat(newDictName, "/new_");
            strcat(newDictName, dB);

        } else {
            strcpy(newDictName, "new_");
            strcat(newDictName, dF);
        }
        // Then output contents
        createNewDict(newDictName);
    }



    strcpy(line, "\n--------DICTIONARY WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);
    printWords(dict->word_list , oFile);

    //printf("\n--------FREED WORDS--------\n");
    strcpy(line, "\n--------FREED WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);
    freeWords(dict->word_list);
    //free dictionary
    free(dict);
    //free m_list
    free(m_list);

    fclose(dFile);
    fclose(iFile);
    fclose(oFile);
    return EXIT_SUCCESS;
}