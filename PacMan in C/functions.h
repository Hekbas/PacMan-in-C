#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#define H 31
#define W 28
#define NR_GHOSTS 4

void displayGame();
void displayReady();
void keyIn();
void newFrame();
void cursorSet(int x, int y);
void hidecursor();

struct coord
{
    int x;
    int y;
};

struct PacMan
{
    struct coord position;
    int vx;
    int vy;
    int vxTurn;
    int vyTurn;
    char shape;
    int lives;
    bool chasing;
    int points;
};

struct Ghost
{
    struct coord position;
    int vx;
    int vy;
    bool chasing;
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
    .lives = 3,
    .chasing = false,
    .points = 0
};

struct Ghost allGhosts[NR_GHOSTS];

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
    {"######·## ######## ##·######"},
    {"######·## #      # ##·######"},
    {"      ·   # M M M#   ·      "},
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

char ready[7][28] =
{
    {"######·##PLAYER ONE##·######"},
    {"######·## ######## ##·######"},
    {"######·## #      # ##·######"},
    {"T     ·   #      #   ·     T"},
    {"######·## #      # ##·######"},
    {"######·## ######## ##·######"},
    {"######·##  READY!  ##·######"},
};

void initialize()
{
    displayGame();
    cursorSet(0, 11);
    displayReady();
    cursorSet(0, 0);

    //  initialize all ghosts
    for (int i = 0; i < NR_GHOSTS; i++)
    {
        allGhosts[i].vx = 0;
        allGhosts[i].vy = 0;
        allGhosts[i].chasing = true;
    }
}

void displayGame()
{
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (playfield[i][j] == '·')
                printf("%c", 250);  // 250 - special char ·
            else
                printf("%c", playfield[i][j]);
        }
        printf("\n");
    }

    printf("\nScore: %d\n", myPacMan.points);
}

void displayReady()
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            if (ready[i][j] == '·')
                printf("%c", 250);  // 250 - special char ·
            else
                printf("%c", ready[i][j]);
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
            myPacMan.vyTurn = -1;
            myPacMan.vxTurn = 0;
            myPacMan.shape = 'v';
            break;
        case 80: case 's': case'S':  // down
            myPacMan.vyTurn = +1;
            myPacMan.vxTurn = 0;
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

void newFrame()
{
    // PACMAN ===========================

    // delete PacMan from old position
    playfield[myPacMan.position.y][myPacMan.position.x] = ' ';

    if (myPacMan.position.x == 0 && myPacMan.position.y == 14 && myPacMan.vx == -1)  //left teleport
        myPacMan.position.x = 27;
    else if (myPacMan.position.x == 27 && myPacMan.position.y == 14 && myPacMan.vx == 1)  //right teleport
        myPacMan.position.x = 0;
    else
    {
        // new coordinates (turn)
        int xTurn = myPacMan.vxTurn + myPacMan.position.x;
        int yTurn = myPacMan.vyTurn + myPacMan.position.y;
        // new coordinates (straight)
        int xStraight = myPacMan.vx + myPacMan.position.x;
        int yStraight = myPacMan.vy + myPacMan.position.y;

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
        myPacMan.position.x += myPacMan.vx;
        myPacMan.position.y += myPacMan.vy;

        // what's in the new location?
        if (playfield[myPacMan.position.y][myPacMan.position.x] == '·')
            myPacMan.points += 10;
        else if (playfield[myPacMan.position.y][myPacMan.position.x] == '+')
            myPacMan.points += 50;
    }

    // put PacMan back again to playfield
    playfield[myPacMan.position.y][myPacMan.position.x] = myPacMan.shape;


    // GHOSTS =========================== pls kill me

    // delete ghosts from positions
    //playfield[allGhosts->vx][] = ' ';
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