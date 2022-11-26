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

Button saveButton,copyButton,pasteButton,fontButton;

int x, y;


void drawIcons()
{
    saveButton.bkcolor = COLOR(177,187,188);
    copyButton.bkcolor = COLOR(177,187,188);
    pasteButton.bkcolor = COLOR(177,187,188);
    fontButton.bkcolor =  COLOR(177,187,188);
    
    saveButton.icon = "icons\\save_icon.gif";
    copyButton.icon = "icons\\copy_icon.gif";
    pasteButton.icon = "icons\\paste_icon.gif";
    
    saveButton.text = "Save";
    copyButton.text = "Copy";
    pasteButton.text = "Paste";
    fontButton.text = "Font";
    
    saveButton.iconWidth = pasteButton.iconWidth = copyButton.iconWidth = 32;
    saveButton.iconHeight = pasteButton.iconHeight = copyButton.iconHeight =  32;
    
    int offset = 5;
    saveButton.buttonWidth = pasteButton.buttonWidth = copyButton.buttonWidth = fontButton.buttonWidth = 125;
    saveButton.buttonHeight = pasteButton.buttonHeight = copyButton.buttonHeight = fontButton.buttonHeight = 40;
    
    saveButton.b.x = offset;
    copyButton.b.x = saveButton.b.x+saveButton.buttonWidth+offset;
    pasteButton.b.x = copyButton.b.x+copyButton.buttonWidth+offset;
    fontButton.b.x = pasteButton.b.x + pasteButton.buttonWidth+offset;
    
    saveButton.b.y = copyButton.b.y = pasteButton.b.y = fontButton.b.y = offset;
    
    drawButton(saveButton);
    drawButton(copyButton);
    drawButton(pasteButton);
    drawButton(fontButton);
}

int font=8; /// 8 - font default, fara niciun offset
void setTextFont(double &lengthError)
{lengthError=1;
 switch (font)
 {
  case 1:  settextstyle(TRIPLEX_FONT,0,0);     lengthError=1.2; break;
  case 2:  settextstyle(SMALL_FONT,0,10);      lengthError=1;   break;
  case 3:  settextstyle(SANS_SERIF_FONT,0,10); lengthError=1;   break;
  case 4:  settextstyle(GOTHIC_FONT,0,0);      lengthError=1.2;   break;
  case 5:  settextstyle(SCRIPT_FONT,0,0);      lengthError=1;   break;
  case 6:  settextstyle(SIMPLEX_FONT,0,0);     lengthError=1.3;   break;
  case 7:  settextstyle(TRIPLEX_SCR_FONT,0,0); lengthError=1;   break;
  case 8:  settextstyle(COMPLEX_FONT,0,0);     lengthError=1;   break;
  case 9:  settextstyle(EUROPEAN_FONT,0,0);    lengthError=1.3;   break;
  case 10: settextstyle(BOLD_FONT,0,0);        lengthError=1.1;   break;
  default: settextstyle(DEFAULT_FONT,0,0);     lengthError=1;   break;
 }
 setcolor(BLACK);
}

void getButtonClick(int x, int y)
{
    Button listOfButtons[] = {copyButton,saveButton,pasteButton,fontButton};
    for(Button b: listOfButtons)
    {
        if(b.b.x <= x && x<= b.b.x + b.buttonWidth && b.b.y <= y && y <= b.b.y + b.buttonHeight)
            cout<<b.text<<" ";
        if(strcmp(b.text,"Font") == 0)
        {
            font = (font+1)%11;
        }
    }
}

void windowsInit()
{
 ///initializare fereastra
    winlenght = 1280;
    winheight = 720;
    initwindow(winlenght, winheight, "Notepad^2");
    setbkcolor(COLOR(221,234,235));
    cleardevice();
    drawIcons();
}

int main()
{
    windowsInit();
    registermousehandler(WM_LBUTTONDOWN,getButtonClick);
    y = saveButton.buttonHeight+10;
    double offsetHeight;
    double offsetLenght;
    double lengthError;
    setTextFont(lengthError);

    curr=getch();
    while (curr!=27) ///escape
          {
           if (curr==13)
              {
               line_index++;
               offsetHeight=textheight(&curr);
               y=y+offsetHeight;
               x=0;
              }
           else
               {
                lines[line_index][col_index[line_index]]=curr;
                ///afisez
                bgiout<<lines[line_index][col_index[line_index]];
                outstreamxy(x,y);
                offsetLenght=textwidth(&curr);
                x=x+lengthError*offsetLenght;
                col_index[line_index]++;
               }
           curr=getch();
          }


    closegraph();
    return 0;
}
