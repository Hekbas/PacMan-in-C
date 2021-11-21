#include "functions.h"

int main()
{
    int freeze = 1000/20;

    system("cls");
    hidecursor();
    initialize();
    Sleep(3000);

    while (status.lives != 0)
    {
        keyIn();
        pacMan();
        ghosts();
        displayGame();
        Sleep(freeze);  
        cursorSet(0, 0);
    }

    gameOver();
}