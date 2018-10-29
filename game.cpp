#include "header.h"

/*
play numbers
0 = quit the game entirely
1 = restart the level
2 = go to the next level
3 = keep playing current level
*/

int main()
{
    char ** fileNames;
    int howManyFiles;
    int keepPlaying;

    findHowManyFiles(MEGAFILENAME, howManyFiles);
    findFileNames(MEGAFILENAME, fileNames, howManyFiles);

    gameClass level[howManyFiles];
    giveLevelsFileNames(level, fileNames, howManyFiles);

    for(int i = 0; i < howManyFiles; ++i)
    {
        keepPlaying = level[i].play();

        //re-play the current level if the user loses, move forward if the user wins
        switch(keepPlaying)
        {
            case 0:
                i = howManyFiles;
                break;
            case 1:
                level[i].constructLevel(fileNames[i], i, howManyFiles); //reset the level
                --i;
                break;
        }
    }

    thankUser();

    deallocateFileNames(fileNames, howManyFiles);
}

//to do:
//tidy up organization of functions etc
//make a tetris-like level
//if the level isn't a perfect square, get program to automatically deal with undefined areas (make them walls?)
//when loading files, do the if loop to make sure it loaded successfully, give error message if not
//maybe also break the loop if it goes past 1,000,000, tell the user they need to reformat the level file
//make a program that makes levels
//dynamically allocate how many levels there are
//delete the bigger functions dynamically allocated stuff
//save what level the user is on
//make a developer mode that allows you to add levels etc
//make the program check that the level is good, e.g. that the user can't go outside the bounds and there is a character etc, so no segmentation fault ever (also what if a letter in the map is not defined?)
//make a system where you can give hints
//make a key of what letters are what
//dynamically allocate how many guys there are
//make it so each X must be hit to win
//if a guy hits a treasure chest and monster simultaneously, he both wins and loses right now
//if a guy pushes against another guy, he doesnt move going down or whatever (fix these)
//to accomplish this, we gotta make an order of operations i guess? that'll be intense but doable, me thinks
//make an array that is a stack of moves to make I guess?
