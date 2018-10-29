#include "header.h"

//void giveLevelName(const char fileNames[])
void gameClass::constructLevel(char * currentLevelName, int currentLevelNumber, int howManyFiles)
{
    if(fileExists(currentLevelName))
    {
        exitLevel = 3;
        howManyGuys = 0;

        levelNumber = currentLevelNumber + 1; //levels start at 1

        howManyLevels = howManyFiles; 

        strcpy(fileName, currentLevelName);

        findRowWidthAndColumnHeight();

        allocateLines();

        loadInFile();

        initializeMaps();

        constructMapLimits();
    }
    else
    {
        cout << "This is where you exit the level." << endl;
        exitLevel = 2;
    }
}

/*
   gameClass::gameClass()
   {
//strcpy(fileName, "level1.txt");

findRowWidthAndColumnHeight();

allocateLines();

loadInFile();

initializeMaps();

}

gameClass::gameClass()
{
}
 */
void gameClass::initializeMaps()
{
    spriteType currentSpriteType;
    sprite currentSprite;

    //declare the arrays of arrays(list of lines)
    mapPosition = new sprite*[columnHeight];
    layerTwoPosition = new sprite*[columnHeight];

    for(int i = 0; i < columnHeight; ++i)
    {
        //declare each line of map
        mapPosition[i] = new sprite[rowWidth];
        layerTwoPosition[i] = new sprite[rowWidth];

        for(int i2 = 0; i2 < rowWidth; ++i2)
        {
            //initialize each spot in each line of the first layer
            currentSpriteType = convertLetterToSpriteType(line[i][i2]);
            initializeSpriteAt(mapPosition[i][i2], currentSpriteType, i, i2);

            //initialize each spot in each line of the second layer
            initializeSpriteAt(layerTwoPosition[i][i2], SECONDLAYERSPRITETYPE, i, i2);
        }
    }
}

void gameClass::printMap()
{
    //in case the map takes the homie out of view, put him back in
    updateMapLimits(myGuy);
    for(int i = upY; i < downY; ++i)
    {
        for(int i2 = leftX; i2 < rightX; ++i2)
            cout << mapPosition[i][i2].rep;
        printInstructions(i); //print the directions next to it as well
        cout << endl;
    }
}

void gameClass::changeOldSpot(sprite & aSprite)
{
    //make the old spot whatever what was previously under it
    //mapPosition[aSprite.X][aSprite.Y] = aSprite;
    mapPosition[aSprite.X][aSprite.Y] = whatsUnder(aSprite);

    //make layer two in the old spot grass
    initializeSpriteAt(layerTwoPosition[aSprite.X][aSprite.Y], grass, aSprite.X, aSprite.Y);
}

void gameClass::changeNewSpot(sprite & aSprite, directionType direction)
{
    sprite newSpot;
    sprite whatWasThere;

    //the spot that the sprite is moving to
    newSpot = mapPosition[aSprite.X][aSprite.Y];

    //handle the next spot if it's a box, monster, grass etc.
    reactToNextSpot(newSpot, aSprite, direction);

    //updated newSpot
    whatWasThere = mapPosition[aSprite.X][aSprite.Y];

    //save whatever is currently in the new spot to the layer beneath
    layerTwoPosition[aSprite.X][aSprite.Y] = whatWasThere;

    //put the character on the top layer
    mapPosition[aSprite.X][aSprite.Y] = aSprite;

    //if a sprite hits the wall, push the sprite back from the wall
    pushBack(aSprite, direction);

    //move the map around
    //updateMapLimits(aSprite);
}

void gameClass::move(sprite & aSprite, directionType direction)
{
    //deal with old spot
    changeOldSpot(aSprite);

    //move the position of where the character will be
    aSprite.move(direction);

    //deal with new spot
    changeNewSpot(aSprite, direction);
}

//deal with winning, losing, starting over, and exiting
int gameClass::play()
{
    cout << endl << "LEVEL " << levelNumber << endl;

    do
    {
        cout << endl;
        printMap();
        getInput();
        doActions();
    }
    while(exitLevel == 3);
    //while(userInput != EXIT);
    return exitLevel;
}

void gameClass::doActions()
{
    //move the homie
    if(userInput == UP)
        move(myGuy, up);
    if(userInput == DOWN)
        move(myGuy, down);
    if(userInput == LEFT)
        move(myGuy, lleft);
    if(userInput == RIGHT)
        move(myGuy, rright);

    if(userInput == STARTOVER)
        exitLevel = 1;
    if(userInput == EXIT)
        exitLevel = 0;
    if(userInput == NEXTLEVEL)
        exitLevel = 2;
}

void gameClass::getInput()
{
    cout << endl;
    cout << "What would you like to do? ";
    cin.get(userInput);
    cin.ignore(100,'\n');
}

void gameClass::printInstructions(int lineNumber)
{
    cout << "     ";
    switch(lineNumber)
    {
        case 0:
            cout << "To move up, press " << UP << ".";
            break;
        case 1:
            cout << "To move left, press " << LEFT << ".";
            break;
        case 2:
            cout << "To move down, press " << DOWN << ".";
            break;
        case 3:
            cout << "To move right, press " << RIGHT << ".";
            break;
        case 4:
            cout << "To start over, press " << STARTOVER << ".";
            break;
        case 5:
            cout << "To exit, press " << EXIT << ".";
            break;
    }
}

sprite gameClass::whatsUnder(sprite & aSprite)
{
    return layerTwoPosition[aSprite.X][aSprite.Y];
}

void gameClass::push(sprite & spriteBeingPushed, directionType direction)
{
    move(spriteBeingPushed, direction);
}

void sprite::move(directionType direction)
{
    if(direction == up)
        --X;

    if(direction == down)
        ++X;

    if(direction == lleft)
        --Y;

    if(direction == rright)
        ++Y;
}

void gameClass::initializeSpriteAt(sprite & aSprite, spriteType typeOfSprite, int startingX, int startingY)
{
    if(typeOfSprite == guy)
        initializeGuyAt(aSprite, startingX, startingY);
    if(typeOfSprite == monster)
        initializeMonsterAt(aSprite, startingX, startingY);
    if(typeOfSprite == box)
        initializeBoxAt(aSprite, startingX, startingY);
    if(typeOfSprite == treasureChest)
        initializeChestAt(aSprite, startingX, startingY);
    if(typeOfSprite == grass)
        initializeGrassAt(aSprite, startingX, startingY);
    if(typeOfSprite == wall)
        initializeWallAt(aSprite, startingX, startingY);
    
}

void gameClass::initializeBoxAt(sprite & aSprite, int startingX, int startingY)
{
    aSprite.X = startingX;
    aSprite.Y = startingY;
    aSprite.rep = BOX;
    aSprite.type = box;
    aSprite.movable = 1;

    mapPosition[aSprite.X][aSprite.Y] = aSprite;
}

void gameClass::initializeGrassAt(sprite & aSprite, int startingX, int startingY)
{
    aSprite.X = startingX;
    aSprite.Y = startingY;
    aSprite.rep = GRASS;
    aSprite.type = grass;
    aSprite.movable = 2;
}

void gameClass::initializeWallAt(sprite & aSprite, int startingX, int startingY)
{
    aSprite.X = startingX;
    aSprite.Y = startingY;
    aSprite.rep = WALL;
    aSprite.type = wall;
    aSprite.movable = 0;
}

void gameClass::initializeGuyAt(sprite & aSprite, int startingX, int startingY)
{
    aSprite.X = startingX;
    aSprite.Y = startingY;
    aSprite.rep = CHARACTER;
    aSprite.type = guy;
    aSprite.movable = 1;

    myGuy = mapPosition[startingX][startingY]; 
    myGuy.X = startingX; //redundant?
    myGuy.Y = startingY;
    
    ++howManyGuys; //count how many guys there are
}

void gameClass::initializeMonsterAt(sprite & aSprite, int startingX, int startingY)
{
    aSprite.X = startingX;
    aSprite.Y = startingY;
    aSprite.rep = MONSTER;
    aSprite.type = monster;
    aSprite.movable = 1;

    //mapPosition[aSprite.X][aSprite.Y] = aSprite;
    //layerTwoPosition[aSprite.X][aSprite.Y] = monster;
}

void gameClass::initializeChestAt(sprite & aSprite, int startingX, int startingY)
{
    aSprite.X = startingX;
    aSprite.Y = startingY;
    aSprite.rep = TREASURECHEST;
    aSprite.type = treasureChest;
    aSprite.movable = 1;

    //mapPosition[aSprite.X][aSprite.Y] = aSprite;
}

sprite gameClass::findSpriteBeingApproached(sprite & aSprite, directionType direction)
{
    if(direction == up)
        return mapPosition[aSprite.X - 1][aSprite.Y];

    if(direction == down)
        return mapPosition[aSprite.X + 1][aSprite.Y];

    if(direction == lleft)
        return mapPosition[aSprite.X][aSprite.Y - 1];

    if(direction == rright)
        return mapPosition[aSprite.X][aSprite.Y + 1];
}


void gameClass::openFile()
{
    fin.open(fileName);
}


void gameClass::loadInFile()
{
    //you should do that if loop to make sure the file loaded successfully!!!
    char temp[rowWidth + 1];
    //openFile();
    fin.open(fileName);
    fin.get();

    //take the file and put it into the lines
    for(int i = 0; i < columnHeight; ++i)
    {
        fin.get(temp, TEMPSIZE, '\n');
        fin.ignore();

        strcpy(line[i], temp);
    }
    fin.close();
    //closeFile();
}

void gameClass::closeFile()
{
    fin.close();
}

spriteType gameClass::convertLetterToSpriteType(char aLetter)
{
    if(aLetter == BOX)
        return box;
    if(aLetter == CHARACTER)
        return guy;
    if(aLetter == GRASS)
        return grass;
    if(aLetter == MONSTER)
        return monster;
    if(aLetter == WALL)
        return wall;
    if(aLetter == TREASURECHEST)
        return treasureChest;
}

void gameClass::findRowWidthAndColumnHeight()
{
    char temp[TEMPSIZE];
    int lineCounter = 0;
    int longestLineLength = 0;
    int currentLineLength;

    //openFile();
    fin.open(fileName);

    fin.get();
    for(int i = 0; !fin.eof(); ++i)
    {
        //find columnHeight
        fin.get(temp, TEMPSIZE, '\n');
        fin.ignore(TEMPSIZE, '\n');
        ++lineCounter;

        //find rowWidth
        currentLineLength = strlen(temp);
        if(currentLineLength > longestLineLength)
            longestLineLength = currentLineLength;
    }

    //make up for the counter going one too many times
    --lineCounter;

    //set the columnHeight and rowWidth for rest of program
    columnHeight = lineCounter;
    rowWidth = longestLineLength;

    //closeFile();
    fin.close();
}

void gameClass::winGame(void)
{
    cout << endl;
    cout << "Yay!!! It's a treasure chest!! YOU BEAT LEVEL " << levelNumber << "!!!!!!!!!" << endl;
    if(levelNumber == howManyLevels)
    {
        cout << endl << "AND, OMG, YOU BEAT THE WHOLE GAME!!" << endl;
        exitLevel = 2;
    }
    else
        askToGoToNextLevel();
}

void gameClass::loseGame(void)
{
    cout << endl;
    cout << "Oh, no! You've been eaten by the monster and so you LOSE!!!!" << endl;
    askToRestart();
}

void gameClass::allocateLines(void)
{
    //there could be an extra line here technically
    line = new char*[columnHeight];
    for(int i = 0; i < columnHeight; ++i)
        line[i] = new char[rowWidth + 1]; 
}

void gameClass::pushBack(sprite & aSprite, directionType direction)
{ 
    direction = reverseDirection(direction);

    if(layerTwoPosition[aSprite.X][aSprite.Y].movable != 2)
        push(aSprite, direction);
}

directionType gameClass::reverseDirection(directionType & direction)
{
    if(direction == up)
        return down;
    if(direction == down)
        return up;
    if (direction == lleft)
        return rright;
    if (direction == rright)
        return lleft;
}

gameClass::~gameClass()
{
    deleteLines();
    deleteMaps();
}

void gameClass::deleteLines()
{
    for(int i = 0; i < columnHeight; ++i)
        delete line[i]; 
    delete line;
}

void gameClass::deleteMaps()
{
    //delete the first and second layers
    for(int i = 0; i < columnHeight; ++i)
    {
        delete mapPosition[i];
        delete layerTwoPosition[i];
    }
    delete mapPosition;
    delete layerTwoPosition;
}

bool gameClass::fileExists(char * fileName)
{
    fin.open(fileName);
    if(fin)
    {
        fin.close();
        return true;
    }
    else
    {
        fin.close();
        return false;
    }
}
void gameClass::askToGoToNextLevel()
{
    char wantToPlayAgain;
    cout << "Would you like to go to the next level? ";

    cin.get(wantToPlayAgain);
    cin.ignore(TEMPSIZE, '\n');

    if(wantToPlayAgain == 'y' || wantToPlayAgain == 'Y')
        exitLevel = 2;
    else if(wantToPlayAgain == 'n' || wantToPlayAgain == 'N')
        exitLevel = 0;
    else
        askToGoToNextLevel();

}
void gameClass::askToRestart()
{
    char wantToPlayAgain;
    cout << "Would you like to try again? ";

    cin.get(wantToPlayAgain);
    cin.ignore(TEMPSIZE, '\n');

    if(wantToPlayAgain == 'y' || wantToPlayAgain == 'Y')
        exitLevel = 1;
    else if(wantToPlayAgain == 'n' || wantToPlayAgain == 'N')
        exitLevel = 0;
    else
        askToRestart();
}

void gameClass::reactToNextSpot(sprite & nextSprite, sprite & currentSprite, directionType direction)
{
    //grass
    if(nextSprite.type == grass)
    {
    }

    //wall
    if(nextSprite.type == wall)
    {
    }

    //box
    if(nextSprite.type == box)
        push(nextSprite, direction);

    //monster
    if(nextSprite.type == monster)
    {
        if(currentSprite.type == guy)
            loseGame();
        else
            push(nextSprite, direction);
    }

    //treasure
    if(nextSprite.type == treasureChest)
    {
        if(currentSprite.type == guy)
            winGame();
        else
            push(nextSprite, direction);
    }

    //guy
    if(nextSprite.type == guy)
    {
        if(currentSprite.type == guy)
            pushBack(nextSprite, direction);
    }
}

void gameClass::constructMapLimits(void)
{
    putHomieInCenter();
}

void gameClass::updateMapLimits(sprite & aSprite)
{
    putHomieInCenter();
}

void gameClass::putHomieInCenter()
{
    int radiusX = MAPSIZEX/2;
    int radiusY = MAPSIZEY/2;

    //if the mapsize is bigger than the actual map, then just use the actual map
    if(MAPSIZEX > rowWidth && MAPSIZEY > columnHeight)
    {
        leftX = 0;
        rightX = rowWidth;
        upY = 0;
        downY = columnHeight;
    }
    else if (MAPSIZEX > rowWidth)
    {
        leftX = 0;
        rightX = rowWidth;
        upY = myGuy.X - radiusY;
        downY = myGuy.X + radiusY;
    }
    else if(MAPSIZEY > columnHeight)
    {
        leftX = myGuy.Y - radiusX;
        rightX = myGuy.Y + radiusX;
        upY = 0;
        downY = columnHeight;
    }
    else
    {
        leftX = myGuy.Y - radiusX;
        rightX = myGuy.Y + radiusX;
        upY = myGuy.X - radiusY;
        downY = myGuy.X + radiusY;
    }

    putInBounds();
}


void gameClass::putInBounds()
{
    
    while(upY < 0 || leftX < 0 || downY > columnHeight || rightX > rowWidth)
    {
        if(upY < 0)
        {
            ++upY;
            ++downY;
        }
        if(leftX < 0)
        {
            ++leftX;
            ++rightX;
        }
        if(downY > columnHeight)
        {
            --upY;
            --downY;
        }
        if(rightX > rowWidth)
        {
            --leftX;
            --rightX;
        }
        
    }
}
