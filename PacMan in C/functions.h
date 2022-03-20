#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>

#define H 31
#define W 28
#define NR_GHOSTS 4

void displayGame();
void displayReady();
void keyIn();
void pacMan();
void statusCheck();
void ghostsChase();
void ghostsScatter();
void displayOver();
void cursorSet(int x, int y);
void hidecursor();

struct coord
{
    int x;
    int y;
};

struct PacMan
{
    struct coord pos;
    int vx;
    int vy;
    int vxTurn;
    int vyTurn;
    char shape;
};

struct Ghost
{
    struct coord pos;
    char posOld;
    int vx;
    int vy;
};

struct Status
{
    int lives;
    int points;
    bool chase;
    bool frightened;

    bool devMode;
};

struct PacMan myPacMan =
{
    {
        .x = 13, 
        .y = 23,
    },
    .vx = 0,
    .vy = 0,
    .vxTurn = -1,
    .shape = '>',
};

struct Ghost red =
{
    {
        .x = 13,
        .y = 11,
    },
    .vx = -1,
    .vy = 0,
};

struct Status status =
{
    .lives = 3,
    .points = 0,
    .chase = true,
    .frightened = false,

    .devMode = true,
};

char playfield[H][W] =
{
    {"############################"},
    {"#············##············#"},
    {"#+####·#####·##·#####·####+#"},
    {"#·####·#####·##·#####·####·#"},
    {"#·####·#####·##·#####·####·#"},
    {"#··························#"},
    {"#·####·##·########·##·####·#"},
    {"#·####·##·########·##·####·#"},
    {"#······##····##····##······#"},
    {"######·##### ## #####·######"},
    {"######·##### ## #####·######"},
    {"######·##          ##·######"},
    {"######·## ###__### ##·######"},
    {"######·## #      # ##·######"},
    {"      ·   # B P Y#   ·      "},
    {"######·## #      # ##·######"},
    {"######·## ######## ##·######"},
    {"######·##          ##·######"},
    {"######·## ######## ##·######"},
    {"######·## ######## ##·######"},
    {"#············##············#"},
    {"#·####·#####·##·#####·####·#"},
    {"#·####·#####·##·#####·####·#"},
    {"#+··##·······  ·······##··+#"},
    {"###·##·##·########·##·##·###"},
    {"###·##·##·########·##·##·###"},
    {"#······##····##····##······#"},
    {"#·##########·##·##########·#"},
    {"#·##########·##·##########·#"},
    {"#··························#"},
    {"############################"},
};

char logic[H][W] =
{
    {"############################"},
    {"#C   LIR    C##C    LIR   C#"},
    {"# ####D##### ## #####D#### #"},
    {"# #### ##### ## ##### #### #"},
    {"#U####U#####U##U#####U####U#"},
    {"#IR  LIRLIRLIRLIRLIRLIR  LI#"},
    {"#D####D##D########D##D####D#"},
    {"# ####U## ######## ##U#### #"},
    {"#C   LI##C  C##C  C##IR   C#"},
    {"######D##### ## #####D######"},
    {"###### #####U##U##### ######"},
    {"###### ##C LIRLIR C## ######"},
    {"###### ## ######## ## ######"},
    {"######U##U#      #U##U######"},
    {"T    LIRLI#      #IRLIR    T"},
    {"######D##D#      #D##D######"},
    {"###### ##U########U## ######"},
    {"###### ##IR      LI## ######"},
    {"###### ##D########D## ######"},
    {"######U##U########U##U######"},
    {"#C   LIRLIR C##C LIRLIR   C#"},
    {"# ####D##### ## #####D#### #"},
    {"# ####U#####U##U#####U#### #"},
    {"#C C##IRLIRLIRLIRLIRLI##C C#"},
    {"### ##D##D########D##D## ###"},
    {"###U## ## ######## ## ##U###"},
    {"#CLIR C##C  C##C  C##C LIRC#"},
    {"# ########## ## ########## #"},
    {"# ##########U##U########## #"},
    {"#C         LIRLIR         C#"},
    {"############################"},
};

char ready[7][28] =
{
    {"######·##PLAYER ONE##·######"},
    {"######·## ######## ##·######"},
    {"######·## #      # ##·######"},
    {"      ·   #      #   ·      "},
    {"######·## #      # ##·######"},
    {"######·## ######## ##·######"},
    {"######·##  READY!  ##·######"},
};

char over[7][28] =
{
    {"###### ##          ## ######"},
    {"###### ## ######## ## ######"},
    {"###### ## #      # ## ######"},
    {"          #      #          "},
    {"###### ## #      # ## ######"},
    {"###### ## ######## ## ######"},
    {"###### ##GAME OVER!## ######"},
};

void initialize()
{
    displayGame();
    cursorSet(0, 11);
    displayReady();
    cursorSet(0, 0);

    // initialize all ghosts ???
}

void statusCheck()
{
    if (status.chase == true)
        ghostsChase();
    else
        ghostsScatter();

    //if(status.frightened == true)
        //ghostsFrightened();
}

void displayGame()
{
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (playfield[i][j] == '#')
                printf("\x1b[34m%c\x1b[0m", playfield[i][j]);
            else if (playfield[i][j] == '·')
                printf("%c", 250);  // 250 - special char ·
            else if (playfield[i][j] == '<' || playfield[i][j] == '>' || playfield[i][j] == 'v' || playfield[i][j] == '^')
                printf("\x1b[1m\x1b[33m%c\x1b[0m", playfield[i][j]);
            else if (playfield[i][j] == 'R')
                printf("\x1b[31m%c\x1b[0m", playfield[i][j]);
            else
                printf("%c", playfield[i][j]);
        }
        printf("\n");
    }

    printf("\nScore: %d\tLives: %d", status.points, status.lives);
}

void displayReady()
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            if (ready[i][j] == '#')
                printf("\x1b[34m%c\x1b[0m", ready[i][j]);
            else if (ready[i][j] == '·')
                printf("%c", 250);  // 250 - special char ·
            else if (i == 0 && j > 8 && j < 19)
                printf("\x1b[1m\x1b[36m%c\x1b[0m", ready[i][j]);
            else if (i == 6 && j > 10 && j < 17)
                printf("\x1b[1m\x1b[33m%c\x1b[0m", ready[i][j]);
            else
                printf("%c", ready[i][j]);
        }
        printf("\n");
    }
}

void displayOver()
{
    cursorSet(0, 11);

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            if (over[i][j] == '#')
                printf("\x1b[34m%c\x1b[0m", over[i][j]);
            else if (over[i][j] == '·')
                printf("%c", 250);  // 250 - special char ·
            else if (i == 6 && j > 8 && j < 19)
                printf("\x1b[31m%c\x1b[0m", over[i][j]);
            else
                printf("%c", over[i][j]);
        }
        printf("\n");
    }
}

void keyIn()
{
    if (_kbhit()) //returns a nonzero value if a key has been pressed. Otherwise, returns 0
    {
        char ch = _getch();

        switch (ch)
        {
        case 72: case 'w': case'W':  // up
            myPacMan.vxTurn = 0;
            myPacMan.vyTurn = -1;
            myPacMan.shape = 'v';
            break;
        case 80: case 's': case'S':  // down
            myPacMan.vxTurn = 0;
            myPacMan.vyTurn = +1;           
            myPacMan.shape = '^';
            break;
        case 75: case 'a': case'A':  // left
            myPacMan.vxTurn = -1;
            myPacMan.vyTurn = 0;
            myPacMan.shape = '>';
            break;
        case 77: case 'd': case'D':  // right
            myPacMan.vxTurn = +1;
            myPacMan.vyTurn = 0;
            myPacMan.shape = '<';
            break;
        }

        //printf("c1=%d\n", ch);
    }
}

void pacMan()
{
    // delete PacMan from old position
    playfield[myPacMan.pos.y][myPacMan.pos.x] = ' ';

    if (myPacMan.pos.x == 0 && myPacMan.pos.y == 14 && myPacMan.vx == -1)  //left teleport
        myPacMan.pos.x = 27;
    else if (myPacMan.pos.x == 27 && myPacMan.pos.y == 14 && myPacMan.vx == 1)  //right teleport
        myPacMan.pos.x = 0;
    else
    {
        // new coordinates (turn)
        int xTurn = myPacMan.vxTurn + myPacMan.pos.x;
        int yTurn = myPacMan.vyTurn + myPacMan.pos.y;
        // new coordinates (straight)
        int xStraight = myPacMan.vx + myPacMan.pos.x;
        int yStraight = myPacMan.vy + myPacMan.pos.y;

        if (playfield[yTurn][xTurn] != '#')    //try turning
        {
            myPacMan.vx = myPacMan.vxTurn;
            myPacMan.vy = myPacMan.vyTurn;
        }
        else if (playfield[yStraight][xStraight] == '#')    //try going straight
        {
            myPacMan.vx = 0;
            myPacMan.vy = 0;
        }

        // update PacMan's coordinate
        myPacMan.pos.x += myPacMan.vx;
        myPacMan.pos.y += myPacMan.vy;

        // what's in the new location?
        if (playfield[myPacMan.pos.y][myPacMan.pos.x] == '·')
            status.points += 10;
        else if (playfield[myPacMan.pos.y][myPacMan.pos.x] == '+')
        {
            status.points += 50;
            status.frightened = true;
        }
    }

    // redraw pacMan
    playfield[myPacMan.pos.y][myPacMan.pos.x] = myPacMan.shape;
}

void ghostsChase()
{
    int x = red.pos.x;
    int y = red.pos.y;

    //delete red ghost from old position
    playfield[y][x] = ' ';

    if (x == myPacMan.pos.x && y == myPacMan.pos.y)  //eaten pacMan?
    {
        red.pos.x = 13;
        red.pos.y = 11;

        red.vx = -1;
        red.vy = 0;

        if (status.frightened == true)
        {
            status.frightened = false;
        }
        else
        {
            status.lives--;
            myPacMan.pos.x = 13;
            myPacMan.pos.y = 23;
        }
    }
    else if (logic[y][x] == 'I')  //chek for intersections
    {
        float distMin = 100;
        char bestPath;

        for (int i = x-1; i < x + 2; i++)
        {
            for (int j = y-1; j < y + 2; j++)
            {
                //check for paths 1 tyle around, excluding the one it came from
                if ((logic[j][i] == 'L' ||
                     logic[j][i] == 'R' ||
                     logic[j][i] == 'U' ||
                     logic[j][i] == 'D') &&
                     logic[j][i] != red.posOld)
                {
                    int xDelta;
                    int yDelta;

                    //find wich path has the shortest distance (straight line) to pacMan
                    if (status.chase == true)
                    {
                        xDelta = i - myPacMan.pos.x;
                        yDelta = j - myPacMan.pos.y;
                    }
                    else
                    {
                        xDelta = i - 0;
                        yDelta = j - 27;
                    }
                    
                    float dist = hypot(xDelta, yDelta); 

                    if (dist < distMin)
                    {
                        distMin = dist;
                        bestPath = logic[j][i];
                    }
                }
            }
        }

        //change ghost speed to follow "best" path
        switch (bestPath)
        {
        case 'L': red.vx = -1; red.vy =  0; break;
        case 'R': red.vx =  1; red.vy =  0; break;
        case 'U': red.vx =  0; red.vy = -1; break;
        case 'D': red.vx =  0; red.vy =  1; break;
        default: break;
        }
    }
    else if (logic[y][x] == 'C')  //chek for corners
    {
        if (red.vx != 0)
        {
            red.vx = 0;

            if (logic[y-1][x] != '#') //check UP
                red.vy = -1;
            else    
                red.vy = 1;
        }
        else
        {
            red.vy = 0;

            if (logic[y][x-1] != '#') //check LEFT
                red.vx = -1;
            else
                red.vx = 1;
        }
    }
    else    //set actual pos as posOld
    {
        switch (logic[y][x])
        {
        case 'L': red.posOld = 'L'; break;
        case 'R': red.posOld = 'R'; break;
        case 'U': red.posOld = 'U'; break;
        case 'D': red.posOld = 'D'; break;
        default: break;
        }
    }

    // update ghost pos
    red.pos.x += red.vx;
    red.pos.y += red.vy;

    // redraw red ghost
    playfield[red.pos.y][red.pos.x] = 'R';
}

void ghostsScatter()
{
    //ghosts goto corners
    //red (0,28)

}

void devMode(bool devMode)
{
    if (devMode == true)
    {
        printf("\n\nPacMan: (%.2d,%.2d)", myPacMan.pos.x, myPacMan.pos.y);
        printf("\nGhost Chasing: %d", status.chase);
        printf("\nGhosts Frightened: %d", status.frightened);
    }
}

// Set cursor position in console
// see http://www.dreamincode.net/forums/topic/153240-console-cursor-coordinates/
void cursorSet(int x, int y)
{
    //Initialize the coordinates
    COORD coord = { x, y };
    //Set the position
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}