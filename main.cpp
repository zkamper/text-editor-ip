#include <iostream>
#include <winbgim.h>
#include <graphics.h>
using namespace std;

int winlenght, winheight;

char lines[100][1024];
char c;
int x, y;

int main()
{   ///initializare fereastra
    winlenght = 1280;
    winheight = 720;
    initwindow(winlenght, winheight, "Notepad^2");
    setbkcolor(COLOR(221,234,235));
    cleardevice();

    setcolor(BLACK);
    c=getch();
    while (c!=27) ///escape
          {

           line(x,y,x+50,y+50);
           x=y=x+10;
           c=getch();
          }



    closegraph();
    return 0;
}
