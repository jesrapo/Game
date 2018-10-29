//make this like tetris?!?!?!?

#include <iostream>
#include <cctype>
#include <cstring>
#include <unistd.h> 
#include <fstream>

using namespace std; 

class sprite;
class gameClass;

enum spriteType {box, guy, grass, wall, monster, treasureChest, nullSprite};
enum directionType {up, down, lleft, rright};

//rando
const int TEMPSIZE = 100;
const char MEGAFILENAME[100] = "megafile.txt";
const spriteType SECONDLAYERSPRITETYPE = grass;
const int MAPSIZEX = 22;
const int MAPSIZEY = 14;

//sprite symbols on map
const char GRASS = '.';
const char MONSTER = 'M';
const char CHARACTER = 'A';
const char BOX = 'B';
const char WALL = 'W';
const char TREASURECHEST = 'X';

//controls
const char UP = 'w';
const char LEFT = 'a';
const char DOWN = 's';
const char RIGHT = 'd';
const char EXIT = 'e';
const char STARTOVER = 'r';
const char NEXTLEVEL = 'n';
  
void findHowManyFiles(const char megaFileName[], int & howManyFiles);
void findFileNames(const char megaFileName[], char ** & fileNames, int howManyFiles);
void deallocateFileNames(char ** & fileNames, int howManyFiles);
void giveLevelsFileNames(gameClass level[], char ** fileNames, int howManyFiles);
void thankUser(void);

class sprite
{
    public:
    int X;
    int Y;
    char rep;

    directionType pushedFrom;
    spriteType type;
    int movable; //0 is like wall, 1 is like box or movable object, 2 is like grass

    void move(directionType direction);
};

class gameClass
{
    public:
        //constructor/destructor
        //gameClass();
        void constructLevel(char * currentLevelName, int currentLevelNumber, int howManyFiles);
        ~gameClass();
        void deleteLines();
        void deleteMaps();

        //set up and print map
        void initializeMaps();
        void printMap();

        //moving mechanics
        void move(sprite & aSprite, directionType direction);
        void push(sprite & spriteBeingPushed, directionType direction);
        sprite whatsUnder(sprite & aSprite);
        void changeNewSpot(sprite & aSprite, directionType direction);
        void changeOldSpot(sprite & aSprite);
        void reactToNextSpot(sprite & nextSprite, sprite & currentSprite, directionType direction);
        void pushBack(sprite & aSprite, directionType direction);

        //overarching gameplay framework
        int play();
        void getInput();
        void doActions();

        //sprite initialization
        void initializeSpriteAt(sprite & aSprite, spriteType typeOfSprite, int startingX, int startingY);
        void initializeBoxAt(sprite & aSprite, int startingX, int startingY);
        void initializeGrassAt(sprite & aSprite, int startingX, int startingY);
        void initializeWallAt(sprite & aSprite, int startingX, int startingY);
        void initializeGuyAt(sprite & aSprite, int startingX, int startingY);
        void initializeMonsterAt(sprite & aSprite, int startingX, int startingY);
        void initializeChestAt(sprite & aSprite, int startingX, int startingY);

        sprite findSpriteBeingApproached(sprite & aSprite, directionType direction);
        void printInstructions(int lineNumber);

        //deal with external data filez
        void openFile();
        void closeFile();
        void findRowWidthAndColumnHeight();
        void loadInFile();
        spriteType convertLetterToSpriteType(char aLetter);
        void allocateLines(void);
        
        //win and lose functions
        void winGame(void);
        void loseGame(void);

        //to be sorted
        directionType reverseDirection(directionType & direction);
        bool fileExists(char * fileName);
        void askToGoToNextLevel();
        void askToRestart();
        void updateMapLimits(sprite & aSprite);
        void constructMapLimits(void);
        void putInBounds(void);
        void putHomieInCenter(void);

    private:
        //the user's move choice
        char userInput;
        int exitLevel;
        
        //the width and height of the map
        int rowWidth;
        int columnHeight;

        //map positions
        sprite ** mapPosition;
        sprite ** layerTwoPosition;
        
        //the main homie
        sprite myGuy;

        //external data file stuff
        ifstream fin;
        ofstream fout;
        char fileName[100]; //change this to being dynamically allocated
        char ** line;

        int levelNumber;
        int howManyLevels;

        int howManyGuys;

        int leftX;
        int rightX;
        int upY;
        int downY;
};
