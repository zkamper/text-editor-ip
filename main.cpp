#include <iostream>
#include <winbgim.h>
#include <graphics.h>
#include "display.h"
#define LMAX 100
#define COLMAX 1024
using namespace std;

int winlenght, winheight;

char lines[LMAX][COLMAX];
int col_index[LMAX];
int line_index=0;
char curr;

Button saveButton,copyButton,pasteButton;

int x, y;


void drawIcons()
{
    saveButton.bkcolor = COLOR(177,187,188);
    copyButton.bkcolor = COLOR(177,187,188);
    pasteButton.bkcolor = COLOR(177,187,188);
    saveButton.icon = "icons\\save_icon.gif";
    copyButton.icon = "icons\\copy_icon.gif";
    pasteButton.icon = "icons\\paste_icon.gif";
    saveButton.text = "Save";
    copyButton.text = "Copy";
    pasteButton.text = "Paste";
    saveButton.iconWidth = pasteButton.iconWidth = copyButton.iconWidth = 32;
    saveButton.iconHeight = pasteButton.iconHeight = copyButton.iconHeight = 32;
    int offset = 5;
    saveButton.buttonWidth = pasteButton.buttonWidth = copyButton.buttonWidth = 125;
    saveButton.buttonHeight = pasteButton.buttonHeight = copyButton.buttonHeight = 40;
    saveButton.b.x = offset;
    copyButton.b.x = saveButton.b.x+saveButton.buttonWidth+offset;
    pasteButton.b.x = copyButton.b.x+copyButton.buttonWidth+offset;
    saveButton.b.y = copyButton.b.y = pasteButton.b.y = offset;
    drawButton(saveButton);
    drawButton(copyButton);
    drawButton(pasteButton);
}


int main()
{   ///initializare fereastra
    winlenght = 1280;
    winheight = 720;
    initwindow(winlenght, winheight, "Notepad^2");
    setbkcolor(COLOR(221,234,235));
    settextstyle(DEFAULT_FONT,0,0);
    cleardevice();
    drawIcons();
    y = saveButton.buttonHeight+10;
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
