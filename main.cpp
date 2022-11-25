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

void drawIcons()
{
  readimagefile("icons\\save_icon.gif",5,5,36,36);
  readimagefile("icons\\copy_icon.gif",46,5,77,36);
  readimagefile("icons\\paste_icon.gif",87,5,118,36);
}



int main()
{   ///initializare fereastra
    winlenght = 1280;
    winheight = 720;
    initwindow(winlenght, winheight, "Notepad^2");
    setbkcolor(COLOR(221,234,235));
    settextstyle(DEFAULT_FONT,0,0);
    cleardevice();
    ///drawIcons();
    setcolor(BLACK);
    curr=getch();
    while (curr!=27) ///escape
          {
           if (curr==13) {line_index++; y=y+textheight("Y")-1; x=0;}
           else
               {
                lines[line_index][col_index[line_index]]=curr;
                ///afisez
                bgiout<<lines[line_index][col_index[line_index]];
                outstreamxy(x,y);
                x=x+textwidth("Y")+2;
                col_index[line_index]++;
               }
           curr=getch();
          }


    closegraph();
    return 0;
}
