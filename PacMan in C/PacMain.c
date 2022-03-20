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
        statusCheck();
        displayGame(0, 0, H, W);
        devMode(status.devMode);
        Sleep(freeze);
        cursorSet(0, 0);
    }

    displayOver();
    getchar();
}