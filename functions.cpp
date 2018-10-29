#include "header.h"

void findHowManyFiles(const char megaFileName[], int & howManyFiles)
{
    int counter = 0;
    char temp[TEMPSIZE];
    fstream fin;
    fin.open(megaFileName);
    fin.get();
    for(int i = 0; !fin.eof(); ++i)
    {
        fin.get(temp, TEMPSIZE, '\n');
        fin.ignore();
        ++counter;
    }
    --counter; //correct for the one extra

    howManyFiles = counter;

    fin.close();
}

void findFileNames(const char megaFileName[], char ** & fileNames, int howManyFiles)
{
    char temp[TEMPSIZE];
    int tempLength;
    fstream fin;
    fin.open(megaFileName);
    fin.get();
    fileNames = new char*[howManyFiles]; 
    for(int i = 0; i < howManyFiles; ++i)
    {
        fin.get(temp, TEMPSIZE, '\n');
        fin.ignore();
        tempLength = strlen(temp);
        fileNames[i] = new char[tempLength + 1];
        strcpy(fileNames[i], temp);
    }

    fin.close();
}

void deallocateFileNames(char ** & fileNames, int howManyFiles)
{
    for(int i = 0; i < howManyFiles; ++i)
        delete fileNames[i];
    delete fileNames;
}

void giveLevelsFileNames(gameClass level[], char ** fileNames, int howManyFiles)
{
    //give each level the name of its file
    for(int i = 0; i < howManyFiles; ++i)
        level[i].constructLevel(fileNames[i], i, howManyFiles);
}

void thankUser(void)
{
    cout << endl << "Thanks for playing!" << endl << endl;
}
