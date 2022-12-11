#include <iostream>
#include <graphics.h>
using namespace std;

int main()
{
    initwindow(100,100);
    settextstyle(SMALL_FONT,0,0);
    setactivepage(1);
    settextjustify(1,1);
    outtextxy(50,50,"Test string");
    getch();
    swapbuffers();
    getch();
}