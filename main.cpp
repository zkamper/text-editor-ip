#include <iostream>
#include <winbgim.h>
#include <graphics.h>
#define LMAX 100
#define COLMAX 1024
using namespace std;

int winlenght, winheight;

char lines[LMAX][COLMAX];
int col_index[LMAX];
int line_index=0;
char curr;

int x, y;

int main()
{   ///initializare fereastra
    winlenght = 1280;
    winheight = 720;
    initwindow(winlenght, winheight, "Notepad^2");
    setbkcolor(COLOR(221,234,235));
    settextstyle(SANS_SERIF_FONT,0,0);
    cleardevice();

    setcolor(BLACK);
    curr=getch();
    while (curr!=27) ///escape
          {
           if (curr==13) {line_index++; y=y+textheight("M"); x=0;}
           else
               {
                lines[line_index][col_index[line_index]]=curr;
                bgiout<<lines[line_index][col_index[line_index]];
                col_index[line_index]++;
                outstreamxy(x,y);
                x=x+textwidth("M")-20;
               }
           curr=getch();
          }


    closegraph();
    return 0;
}
