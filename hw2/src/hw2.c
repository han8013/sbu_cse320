#include "hw2.h"

/* Great filename. */

void processDictionary(FILE* f){
    dict->num_words = 0;
    while(!feof(f))
    {
        //initialize the current word.
        struct dict_word* currWord;
        if((currWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
        {
            printf("OUT OF MEMORY.\n");
            return;
        }
        currWord->num_misspellings = 0;
        currWord->misspelled_count = 0;

        //variables
        char word[MAX_SIZE];
        char* wdPtr = word;
        char line[MAX_SIZE];
        char* character = line;
        //char word_list[MAX_MISSPELLED_WORDS+1][MAX_SIZE]; // XINGHAN
        memset(word, 0, MAX_SIZE+1);
        memset(line, 0, MAX_SIZE+1);

        int counter = 0;
        int firstWord = 1;

        fgets(line, MAX_SIZE+1, f);
        //Avoid empty line

        if(strlen(line) != 0){
            //if there isn't a space at the end of the line, put one there
            if((line[strlen(line)-2] != ' ' && line[strlen(line)-1] == '\n') || (line[strlen(line)-1] != ' ' && line[strlen(line)-1] != '\n'))
                strcat(line, " ");

            while(*character != '\0')  // XINGHAN
            {
                if(counter >= MAX_MISSPELLED_WORDS+1)
                    break;
                //if the character is a space, add the word in word_list and make word NULL.
                if(*character == ' ')
                {
                    wdPtr = NULL;  // *wdPtr = NULL; XINGHAN
                    wdPtr = word;
                    if(firstWord)
                    {
                        addWord(currWord, wdPtr);
                        dict->num_words++;

                        firstWord = 0;
                        memset(word, 0, MAX_SIZE+1);
                    }
                    else
                    {
                        struct misspelled_word* currMisspelling;
                        if((currMisspelling = malloc(sizeof(struct misspelled_word))) == NULL)
                        {
                            printf("ERROR: OUT OF MEMORY.");
                            return;
                        }

                        addMisspelledWord(currMisspelling, currWord, wdPtr);
                        memset(word, 0, MAX_SIZE+1);
                    }
                }
                //if the character isn't a space or a new line, add the character to word.
                else if(*character != '\n')
                    *(wdPtr++) = *character;
                character++;
            }
        }
    }
}

void addWord(struct dict_word* dWord, char* word){
    //setting up dWord fields
    dWord->misspelled_count = 0;
    dWord->num_misspellings = 0;
    dWord->next = dict->word_list;
    strcpy(dWord->word, word);
    dict->word_list = dWord;
}

void addMisspelledWord(struct misspelled_word* misspelledWord, struct dict_word* correctWord, char* word){
    //setting up misspelledWord fields
    strcpy(misspelledWord->word, word);
    misspelledWord->misspelled = 0;
    misspelledWord->correct_word = correctWord;
    misspelledWord->next = m_list;
    (correctWord->misspelled)[correctWord->num_misspellings++] = misspelledWord;
    //correctedWord->++;
    m_list = misspelledWord;
}

void freeWords(struct dict_word* currWord){
    if(currWord != NULL)
    {
        freeWords(currWord->next);

        //free word
        printf("FREED %s\n", currWord->word);
        free(currWord);
    }
}

void printWords(struct dict_word* currWord){ //, FILE* f){ XINGHAN
    // Old Style; XINGHAN
    /*if(currWord != NULL)
    {
        printWords(currWord->next, f);

        char line[MAX_SIZE];
        int i;

        sprintf(line, "%s\n", currWord->word);
        fwrite(line, strlen(line)+1, 1, f);

        sprintf(line, "\tNUMBER OF TIMES WORD IS MISSPELLED: %d\n", currWord->misspelled_count); // puts string into buffer
        fwrite(line, strlen(line)+1, 1, f);

        sprintf(line, "\tNUMBER OF MISSPELLINGS: %d\n", currWord->num_misspellings);
        fwrite(line, strlen(line)+1, 1, f);

        for(i = 0; i<currWord->num_misspellings; i++)
        {
            sprintf(line, "\tMISPELLED WORD #%d: %s\n", i,((currWord->misspelled)[i])->word);
            fwrite(line, strlen(line)+1, 1, f);

            sprintf(line,"\t\tMISPELLED?: %d\n", ((currWord->misspelled)[i])->misspelled);
            fwrite(line, strlen(line)+1, 1, f);

            sprintf(line, "\t\tACTUAL WORD: %s\n", ((currWord->misspelled)[i])->correct_word->word); //XINGHAN
            fwrite(line, strlen(line)+1, 1, f);

            if(((currWord->misspelled)[i])->next->word != NULL)
            {
                sprintf(line, "\t\tNEXT MISPELLED WORD: %s\n", ((currWord->misspelled)[i])->next->word);
                fwrite(line, strlen(line)+1, 1, f);
            }
        }

        if((currWord->next)->word != NULL)
        {
            sprintf(line,"\tNEXT WORD: %s\n", (currWord->next)->word);
            fwrite(line, strlen(line)+1, 1, f);
        }
    }*/

    // New Style
    char top1[MAX_SIZE]; //Top misspelled word number 1
    char top2[MAX_SIZE]; //Top misspelled word number 2
    char top3[MAX_SIZE]; //Top misspelled word number 3
    memset(top1, 0, MAX_SIZE+1);
    memset(top2, 0, MAX_SIZE+1);
    memset(top3, 0, MAX_SIZE+1);
    int max1 = 0;
    int max2 = 0;
    int max3 = 0;
    int total = 0; // Total number of words in dictionary
    int dSize = sizeof(dict); //Size of dictionary (in bytes)
    int mSize = 0; //Size of misspelled word list (in bytes)
    int num_mis = 0; //Total number of misspelled words: 5
    if(currWord != NULL)
    {
        total ++;
        dSize += sizeof(currWord);

        //char line[MAX_SIZE];
        int i;



        // update top 3 misspelled if necessary
        if(currWord->misspelled_count >= max1) {
            max3 = max2;
            memset(top3, 0, MAX_SIZE+1);
            strcpy(top3, top2);
            max2 = max1;
            memset(top2, 0, MAX_SIZE+1);
            strcpy(top2, top1);
            max1 = currWord->misspelled_count;
            memset(top1, 0, MAX_SIZE+1);
            strcpy(top1, currWord->word);

        } else if (currWord->misspelled_count >= max2) {
            max3 = max2;
            memset(top3, 0, MAX_SIZE+1);
            strcpy(top3, top2);
            max2 = currWord->misspelled_count;
            memset(top2, 0, MAX_SIZE+1);
            strcpy(top2, currWord->word);


        } else if (currWord->misspelled_count >= max3) {
            max3 = currWord->misspelled_count;
            memset(top3, 0, MAX_SIZE+1);
            strcpy(top3, currWord->word);

        }

        for(i = 0; i<currWord->num_misspellings; i++)
        {
            num_mis++;
            mSize += sizeof((currWord->misspelled)[i]);
            //sprintf(line, "\tMISPELLED WORD #%d: %s\n", i,((currWord->misspelled)[i])->word);
            //fwrite(line, strlen(line)+1, 1, f);

            //sprintf(line,"\t\tMISPELLED?: %d\n", ((currWord->misspelled)[i])->misspelled);
            //fwrite(line, strlen(line)+1, 1, f);

            //sprintf(line, "\t\tACTUAL WORD: %s\n", ((currWord->misspelled)[i])->correct_word->word); //XINGHAN
            //fwrite(line, strlen(line)+1, 1, f);

            if(((currWord->misspelled)[i])->next->word != NULL)
            {
                //sprintf(line, "\t\tNEXT MISPELLED WORD: %s\n", ((currWord->misspelled)[i])->next->word);
                //fwrite(line, strlen(line)+1, 1, f);
            }
        }

        if((currWord->next)->word != NULL)
        {
            //sprintf(line,"\tNEXT WORD: %s\n", (currWord->next)->word);
            //fwrite(line, strlen(line)+1, 1, f);
        }
    }


}

void processWord(char* inputWord){
    if(foundMisspelledMatch(inputWord))
        return;
    if(foundDictMatch(inputWord))
        return;
    else
    {   /*
        // Old Style, XINGHAN
        // Scanning Y/N
        char ch;
        char conf;

        do
        {
            printf("\"%s\" was not found in the dictionary. Do you want to add it (Y/N)? ", inputWord);
            scanf("%c", &conf);
            while ((ch = getchar()) != '\n' && ch != EOF);
        }while(conf!='Y' && conf!='N');

        // handle Y/N
        if(conf == 'Y')
        {
            struct dict_word* newWord;
            //int counter = 0; // XINGHAN

            if((newWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
            {
                printf("ERROR: OUT OF MEMORY.\n");
                return;
            }

            addWord(newWord, inputWord);
            dict->word_list = newWord;
            printf("Added \"%s\" to Dictionary. Add misspellings (Y/N)? ", inputWord);

            do
            {
                scanf("%c", &conf);
                while ((ch = getchar()) != '\n' && ch != EOF);
            }while(conf!='Y' && conf!='N');

            if(conf=='Y')
            {
                int numMisspellings=0;
                do
                {
                    printf("How many misspellings (1-5)?");
                    scanf("%d", &numMisspellings);
                    while ((ch = getchar()) != '\n' && ch != EOF);
                }while(numMisspellings<1 || numMisspellings>5);

                while(numMisspellings > 0)
                {
                    char word[WORDLENGTH];
                    char* wdPtr = word;
                    struct misspelled_word* newMWord;

                    if((newMWord = (struct misspelled_word*) malloc(sizeof(struct misspelled_word))) == NULL)
                    {
                        printf("ERROR: OUT OF MEMORY.");
                        return;
                    }
                    printf("Enter misspelling: ");
                    scanf("%s", word);
                    addMisspelledWord(newMWord, newWord, wdPtr);
                    printf("Misspelling added\n");
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    numMisspellings--;
                }
            }
        }*/

        // New Style
        // First add new word if -A is specified
        if(aFlag == 1) {
            struct dict_word* newWord;
            if((newWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
            {
                printf("ERROR: OUT OF MEMORY.\n");
                return;
            }

            addWord(newWord, inputWord);
            dict->word_list = newWord;
            // Update dFlag since the dictionary is modified
            dFlag = 1;

            //printf("Added \"%s\" to Dictionary.", inputWord);

            // Then add misspelled words if necessary
            if(nMis > 0 && nMis < 6) {
                int numMisspellings = nMis;
                // Generate n misspelled words
                char** typos = gentypos(numMisspellings, inputWord);

                while(numMisspellings > 0)
                {
                    char word[WORDLENGTH];
                    char* wdPtr = word;
                    struct misspelled_word* newMWord;

                    if((newMWord = (struct misspelled_word*) malloc(sizeof(struct misspelled_word))) == NULL)
                    {
                        printf("ERROR: OUT OF MEMORY.");
                        return;
                    }
                    strcpy(word, typos[--numMisspellings]);
                    addMisspelledWord(newMWord, newWord, wdPtr);
                }
            }
        }
    }
}

bool foundMisspelledMatch(char* inputWord){
    struct misspelled_word* listPtr = m_list;
    while(listPtr != NULL)
    {
        if(strcasecmp(inputWord, listPtr->word) == 0)
        {
            strcpy(inputWord, listPtr->correct_word->word);
            listPtr->misspelled = 1;
            listPtr->correct_word->misspelled_count++;
            return true;
        }
        listPtr = listPtr->next; // XINGHAN
    }
    return false;
}

bool foundDictMatch(char* inputWord){
    struct dict_word* listPtr = dict->word_list;
    while(listPtr != NULL)
    {
        if(strcasecmp(inputWord, listPtr->word) == 0)
            return true;
        listPtr = listPtr->next;
    }
    return false;
}

void createNewDict(char* dictName) {
    //puts(dictName);
    FILE* newDict = fopen(dictName, "w");
    //FILE* newDict = fopen(stdout, "w");
    struct dict_word* dwPtr = dict->word_list;
    char word[MAX_SIZE];
    while(dwPtr != NULL) {
        // write the word
        memset(word, 0, MAX_SIZE + 1);
        strcpy(word, dwPtr->word);
        fwrite(word, strlen(word)+1, 1, newDict);

        // write misspells
        int i = 0;
        while(i < dwPtr->num_misspellings) {
            struct misspelled_word* mwPtr = dwPtr-> misspelled[i++];
            memset(word, 0, MAX_SIZE + 1);
            strcpy(word, " ");
            fwrite(word, strlen(word)+1, 1, newDict);
            memset(word, 0, MAX_SIZE + 1);
            strcpy(word, mwPtr->word);
            fwrite(word, strlen(word)+1, 1, newDict);
            //printf("heeee  %s\n", mwPtr->word);
        }

        // write new line
        memset(word, 0, MAX_SIZE + 1);
        strcpy(word, "\n");
        fwrite(word, strlen(word)+1, 1, newDict);

        // next word
        dwPtr = dwPtr->next;
    }
}