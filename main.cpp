#include <iostream>
#include <winbgim.h>
#include <graphics.h>
#include "display.h"
#define NMAX 10000
using namespace std;

double lengthError;
Button saveButton,copyButton,pasteButton,fontButton;

void getButtonClick(int x,int y);

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

    registermousehandler(WM_LBUTTONDOWN,getButtonClick);
}

int font=8; /// 8 - font recomandat, fara niciun offset
void setTextFont()
{lengthError=1;
 switch (font)
 {
  case 1:  settextstyle(TRIPLEX_FONT,0,0);     lengthError=1.2; break;
  case 2:  settextstyle(SMALL_FONT,0,10);      lengthError=1;   break;
  case 3:  settextstyle(SANS_SERIF_FONT,0,10); lengthError=1;   break;
  case 4:  settextstyle(GOTHIC_FONT,0,0);      lengthError=1.2; break;
  case 5:  settextstyle(SCRIPT_FONT,0,0);      lengthError=1;   break;
  case 6:  settextstyle(SIMPLEX_FONT,0,0);     lengthError=1.3; break;
  case 7:  settextstyle(TRIPLEX_SCR_FONT,0,0); lengthError=1;   break;
  case 8:  settextstyle(COMPLEX_FONT,0,0);     lengthError=1;   break;
  case 9:  settextstyle(EUROPEAN_FONT,0,0);    lengthError=1.3; break;
  case 10: settextstyle(BOLD_FONT,0,0);        lengthError=1.1; break;
  default: settextstyle(DEFAULT_FONT,0,0);     lengthError=1;   break;
 }
 setcolor(BLACK);
}

void getButtonClick(int x, int y)
{
    Button b[] = {copyButton,saveButton,pasteButton,fontButton};
    int buttCount = 4;
    for(int i = 0 ; i < buttCount;i++)
    {
        if(b[i].b.x <= x && x<=b[i].b.x+b[i].buttonWidth && b[i].b.y <= y && y<=b[i].b.y+b[i].buttonHeight)
            cout<<b[i].text<<" ";
        if(strcmp(b[i].text,"Font")==0)
        {
            font=(font+1)%11;
            setTextFont();
        }
    }
}

int winLength, winHeight;
void windowsInit()
{
 ///initializare fereastra
    winLength = 1280;
    winHeight = 720;
    initwindow(winLength, winHeight, "Notepad^2");
    setbkcolor(COLOR(221,234,235));
    cleardevice();
    drawIcons();
}


char text[NMAX];
int lgtext;
double offsetHeight, offsetLength;
double currWordLength=0;
int currWordStart=0;
int x=8, y;

void writeText(int left, int right)
{
 char curr;
 setTextFont();
 for (int i=left; i<=right; i++) {
 offsetLength=textwidth(text+i);
 offsetHeight=textheight(text+i);
 curr=text[i];
 if (curr==13)
    {
     y+=offsetHeight; x=8;
     currWordLength=currWordStart=0;
    }
 else
    {
     if (x+lengthError*offsetLength+8<=winLength)
        {
         bgiout<<curr;
         outstreamxy(x,y);
         x+=lengthError*offsetLength;
         currWordLength+=lengthError*offsetLength;
         if (curr==32) {currWordLength=0; currWordStart=i+1; cout<<1;}
        }
     else if (currWordLength+lengthError*offsetLength+16>winLength)
             {
              y=y+offsetHeight; x=8;
              bgiout<<curr;
              outstreamxy(x,y);
              x+=lengthError*offsetLength;
              currWordLength=lengthError*offsetLength;
              currWordStart=lgtext;
             }
          else
              {
               cleardevice();
               drawIcons();
               x=8, y=saveButton.buttonHeight+10;
               currWordLength=0;
               writeText(0,currWordStart-1);
               y+=offsetHeight; x=8;
               writeText(currWordStart,lgtext);
               currWordLength+=lengthError*offsetLength;
              }
    }
 }
}

void readText()
{
    char curr;
    curr=getch();
    while (curr!=27) ///escape
    {   text[lgtext]=curr;
        writeText(lgtext, lgtext);
        lgtext++;
        curr=getch();
    }

}

int main()
{
    windowsInit();
    y=saveButton.buttonHeight+10;
    readText();
    closegraph();
    return 0;
}
