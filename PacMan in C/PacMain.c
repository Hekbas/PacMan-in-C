#include "functions.h"

int main()
{
    int freeze = 1000/15;

    system("cls");
    hidecursor();
    initialize(allGhosts);
    Sleep(3000);

    while (1)
    {
        keyIn();
        newFrame();
        displayGame();

        Sleep(freeze);
        cursorSet(0, 0);
    }
}