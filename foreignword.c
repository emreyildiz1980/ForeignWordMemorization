#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

/* Global Variables */
COORD coord = {0, 0}; // top-left corner of window

/**
       function : gotoxy
       @param input: x and y coordinates
       @param output: moves the cursor in specified position of the console */

void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/** Main function starting */

int main()
{
    FILE *fp, *ft; // file pointers
    char another, choice;

    /** structure that represent a word */
    struct word
    {
        char name[50];            //name of the word
        char meaningEng[200];     // word meaning in English
        char meaningFor[100];     // word meaning in your Native Language(for example Turkish)
        char wordinsentence[200]; // write a sentence with this word
    };

    struct word w; // structure variable creation

    char wordname[50]; // string to store name of the word

    long int recsize; // size of each record of word

    /** open the file in binary read and write mode
     * if the file WORD.DAT already exist then it open that file read-write mode
     * if the file doesn't exist, it simply create a new copy
     */
    fp = fopen("WORD.DAT", "rb+");
    if (fp == NULL)
    {
        fp = fopen("WORD.DAT", "wb+");
        if (fp == NULL)
        {
            printf("Can not open file");
            exit(1);
        }
    }

    /// size of each record
    recsize = sizeof(w);

    /// infinite loop continues until the break statement encounter
    while (1)
    {
        system("cls");         ///clear the console window
        gotoxy(30, 10);        /// move the cursor to position 30, 10 from top left corner
        printf("1. Add Word"); /// option for add word
        gotoxy(30, 12);
        printf("2. List Words"); /// option for showing existing words
        gotoxy(30, 14);
        printf("3. Modify Words"); /// option for editing  words
        gotoxy(30, 16);
        printf("4. Delete Words"); /// option for deleting  words
        gotoxy(30, 18);
        printf("5. Random Word"); /// option for displaying random  word
        gotoxy(30, 20);
        printf("6. EXIT"); /// exit from the program
        gotoxy(30, 22);
        printf("Your Choice: "); /// enter the choice 1, 2, 3, 4, 5, 6
        fflush(stdin);           /// flush the input buffer
        choice = getche();       /// get the input from keyboard
        switch (choice)
        {
        case '1': /// if user presses 1
            system("cls");
            fseek(fp, 0, SEEK_END); /// search the file and move the cursor to end of the file
            /// here 0 indicates moving 0 distance from the end of the file

            another = 'y';
            while (another == 'y')
            {
                printf("\nEnter word: ");
                gets(w.name);
                printf("\nEnter the meaning of the word in English: ");
                gets(w.meaningEng);
                printf("\nEnter the meaning of the word in your native language: ");
                gets(w.meaningFor);
                printf("\nEnter a sentence with this word in English: ");
                gets(w.wordinsentence);

                fwrite(&w, recsize, 1, fp);

                printf("\nAdd another word(y/n) ");
                fflush(stdin);
                another = getche();
            }
            break;
        case '2':
            system("cls");
            rewind(fp); /// this moves file cursor to the start of the file
            int num = 1;
            while (fread(&w, recsize, 1, fp) == 1) /// read the file and fetch the record one record per fetch
            {
                printf("\n%d. %s : %s", num++, w.name, w.meaningFor); //  print the name and the meaning for your native language
            }
            getch();
            break;

        case '3': /// if user presses 3 then do editing existing word
            system("cls");
            another = 'y';
            while (another == 'y')
            {
                printf("Enter the word name to modify: ");
                gets(wordname);
                rewind(fp);
                while (fread(&w, recsize, 1, fp) == 1) /// fetch all words from file
                {
                    if (strcmp(w.name, wordname) == 0) ///if entered name matches with the one in the file
                    {
                        printf("\nEnter NEW name: ");
                        gets(w.name);
                        printf("\nEnter the NEW meaning of the word in English: ");
                        gets(w.meaningEng);
                        printf("\nEnter the NEW meaning of the word in your native language: ");
                        gets(w.meaningFor);
                        printf("\nEnter a NEW sentence with this word in English: ");
                        gets(w.wordinsentence);
                        fseek(fp, -recsize, SEEK_CUR); /// move the cursor 1 step back from the curent position
                        fwrite(&w, recsize, 1, fp);    ///  override the word
                        break;
                    }
                }
                printf("\nModify another word(y/n)");
                fflush(stdin);
                another = getche();
            }
            break;
        case '4':
            system("cls");
            another = 'y';
            while (another == 'y')
            {
                printf("\nEnter name of word to delete: ");
                gets(wordname);
                ft = fopen("temp.dat", "wb"); /// create a file for temporary storage
                rewind(fp);
                while (fread(&w, recsize, 1, fp) == 1) /// read all words from file
                {
                    if (strcmp(w.name, wordname) != 0) /// if the entered name matches with a word or not
                    {
                        fwrite(&w, recsize, 1, ft); /// move all words except the one that is deleted to the temp file
                    }
                }
                fclose(fp);
                fclose(ft);
                remove("WORD.DAT");             /// remove the original file
                rename("temp.dat", "WORD.DAT"); /// rename the temp file with the original name
                fp = fopen("WORD.DAT", "rb+");
                printf("Delete another word(y/n)");
                fflush(stdin);
                another = getche();
            }
            break;
        case '5':
            system("cls");
            rewind(fp);
            int wordcount = 0, rnd = 0;
            if (fp != NULL)
            {
                while (fread(&w, recsize, 1, fp) == 1)
                {
                    ++wordcount;
                }
                if (wordcount != 0)
                {
                    rnd = rand() % wordcount;
                    rewind(fp);
                    fseek(fp, recsize * rnd, SEEK_SET);
                    fread(&w, recsize, 1, fp);
                    printf("\n\n%s : \n\n ", w.name);
                    printf("\t\t%s\n\n ", w.meaningEng);
                    printf("\t\t%s\n\n ", w.meaningFor);
                    printf("\t\t\t***  %s\n\n ", w.wordinsentence);
                }
            }

            getch();
            break;
        case '6':
            fclose(fp);
            exit(0);
        }
    }
    return 0;
}
